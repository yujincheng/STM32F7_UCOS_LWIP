---
layout: post
title: STM32 + LWIP + UCOS 实现 webserver
date: 2018-8-3
author: 阿金
cover: https://www.micrium.com/wp-content/themes/micrium_theme_2017/images/product_logos/product_logo_ucos.png
tags: 嵌入式
---

> 该项目主要完成在嘉兴爱迪曼（天控科技）的主控芯片配置，使用的开发板为 ALIENTEK STM32F7 开发板。

## 项目需求

在单片机嵌入式系统中，没有外界FLASH，纯粹在MCU中，利用片上资源，实现webserver，实现像路由器一样，通过网页对MCU中的各子功能进行配置。

在这一节中，**主要讨论把网页的数据写入MCU的全局变量，并且将该全局变量从内存中读出来显示在网页上。**

## 环境配置

开发环境为 keil uvision5，在windows 下开发。

整个项目托管在 coding.net 上。可以通过git clone 获取。

```shell
    git clone https://git.coding.net/yujincheng/STM32F7_UCOS_LWIP.git
    cd STM32F7_UCOS_LWIP
    git checkout 9d4963a540c6baac4eec614c00b598d9878e7685
```

用 keil 打开 USER//NETWORK.uvprojx 文件。

## 网页前端

前端本来就是一个 html，但是显示不能把html文件直接放到STM32里面。在这里，需要通过 makefsdata.exe 把原始网页进行转化。来添加到工程中。

### makefsdata 使用

makefsdata.exe 工具位于 LWIP\lwip_app\web_server_demo\makefsdata 目录下。在该目录下，需要关心三个文件/路劲。

1. makefsdata.exe 是工具，会把fs目录中的网站转化成 fsdata.c。双击运行。
2. fs 目录，在这里存放整个网站。需要网站足够简单。理论上支持一个静态网页已经很费劲了。如果要支持动态网站，需要用shtml这类文件来实现。之后会讲。
3. fsdata.c 是生成的网站的C文件，每次生成新的前端之后，需要用改文件替换编译用的网站文件。编译用的网站文件为：LWIP\lwip_app\web_server_demo\fsdata.c

将网站放进 fs 目录中，双击 makefsdata.exe 可以回产生一个 fsdata.c 文件，将 LWIP\lwip_app\web_server_demo\fsdata.c 文件用新产生的 fsdata.c 替换，再编译工程即可。

### shtml简介

shtml 语法和 html 很像。基于SSI（Server Side Include，服务器端嵌入）。服务器将 "<!-- -->" 之间的内容进行理解和嵌入。对，这个正是注释符号，如果服务器法理解 "<!-- -->" 之间的内容，就会自动忽略这些信息。

在 LWIP\lwip_app\web_server_demo\makefsdata\fs\index.shtml 文件中，可以看到服务器端嵌入的用法。例如：文件第 31和 第36行。对应代码段的第3 和 第8 行。

```html

  <p>
  测试显示内存字符
  <!--#0-->
  </p>

  <p>
  测试显示输入后回显示
  <!--#1-->
  </p>

```

```<!--#0-->```和```<!--#1-->``` 表示服务器在这里需要嵌入内容。标记为'0'和'1'

在后端部分，会设置填充方式。


### cgi简介

为了简单理解，就把cgi理解成处理用户交互的程序。

在 LWIP\lwip_app\web_server_demo\makefsdata\fs\index.shtml 中，第 14 和 第 22 行，下面代码段的第3 和 第10行，指定了表单需要被如何处理。两个表单分别被 ```leds.cgi``` 和 ```ip.cgi``` 处理。

```html

<p>
  测试表单 form_buttom
  <form method="get" action="/leds.cgi">
    LED1:
       <input type="radio" name="LED1" value="LED1ON" id="LED1_0">ON
       <input name="LED1" type="radio" id="LED1_1" value="LED1OFF">OFF<br>
   <br>
   <input type="submit" value="LED"> // 点击此按钮，会访问网址 10.10.10.30/leds.cgi?LED1=LED1ON (或者 LED1=LED1OFF,取决于选择的radio，假设板子的IP地址为 10.10.10.30)
  </form>

  <form method="get" action="/ip.cgi">
    <input type="text" name="IPaddr">
   <br>
   <input type="submit" value="IP">
  </form>
  </p>

```

在后端部分，会设置这两个 cig 文件的关联函数。


至此，网页前端需要注意的就这些。


## 服务器后端

所谓服务器后端，主要指的是C代码，对需要交互的地方进行响应。

### CGI 反馈

在文件 LWIP\lwip_app\web_server_demo\httpd_cgi_ssi.c 中对CGI的交互进行了定义。

其中 47-51 行，定义 cgi 方法对应的句柄回调函数。具体的句柄实现见后。

```C

static const tCGI ppcURLs[]= //cgi程序
{
  {"/leds.cgi",LEDS_CGI_Handler},
  {"/ip.cgi",IP_CGI_Handler},
};

```

并且在初始化时对回调函数进行绑定（文件中 161-166 行）

```C

//CGI句柄初始化
void httpd_cgi_init(void)
{
  //配置CGI句柄
  http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}

```

具体的两个处理大同小异，根据功能确定。下面以LED配置为例讲解。

```C

//CGI LED控制句柄
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  u8 i=0;  //注意根据自己的GET的参数的多少来选择i值范围
  iIndex = FindCGIParameter("LED1",pcParam,iNumParams);  //找到led的索引号
  //只有一个CGI句柄 iIndex=0
  if (iIndex != -1)
  {
    LED1(1);  //关闭LED1灯
    for (i=0; i<iNumParams; i++) //检查CGI参数
    {
      if (strcmp(pcParam[i] , "LED1")==0)  //检查参数"led" 属于控制LED1灯的
      {
      if(strcmp(pcValue[i], "LED1ON") ==0)  //改变LED1状态
        LED1(0); //打开LED1
      else if(strcmp(pcValue[i],"LED1OFF") == 0)
        LED1(1); //关闭LED1
      }
    }
   }
  return "/index.shtml";
}

```

传入的四个参数为
1. iIndex： 句柄ID，在之后会覆盖，没什么卵子用。
2. iNumParams： get方法参数的个数。
3. pcParam： 参数名
4. pcValue： 参数值

以 上一章节中 shtml cgi 控制 LED 为例。

当访问 ```10.10.10.30/leds.cgi?LED1=LED1ON``` 时。leds.cgi 对应的句柄函数，LEDS_CGI_Handler 会被调用。

这时，iNumParams == 1, pcParam[0] == "LED1", pcValue[0] == "LED1ON"

通过分支（if）语句，可以对不同的值进行操作。


### SSI 填充

首先，定义 tag，该tag 需与 shtml 中对应。在文件 LWIP\lwip_app\web_server_demo\httpd_cgi_ssi.c 中 39-44 行。

```C

static const char *ppcTAGs[]=  //SSI的Tag
{
  "0", // <!--#0-->
  "1", // <!--#1-->
  "2", // <!--#2-->
  "3"  // <!--#3-->
};

```

并且在初始化时绑定 ppcTAGs，在 154-160 行。

```C

//SSI句柄初始化
void httpd_ssi_init(void)
{  
  //配置SSI句柄
  http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS); .//绑定回调函数为 SSIHandler, shtml中TAG为 ppcTAGs, NUM_CONFIG_SSI_TAGS为TAG的数目。
}

```

最后，只需要定义回调函数 SSIHandler 即可。在 90-104 行。

```C

static u16_t SSIHandler(int iIndex,char *pcInsert,int iInsertLen)
{
  switch(iIndex) // 该数据的顺序与 ppcTAGs 一致，即 <!--#0--> 对应 0, <!--#1--> 对应 1
  {
    case 0:
        TEST_Handler(pcInsert); //简单句柄见下
        break;
    case 1:
        TEST1_Handler(pcInsert);
        break;
  }
  return strlen(pcInsert);
}

```

其中几个参数意义如下：
1. iIndex，区分 shtml 的需填充位置。
2. pcInsert，为需填充部分的字符串指针，直接修改该指针对象的区域的字符串即可。
3. iInsertLen 好像没啥用

TEST_Handler(pcInsert) 函数非常简单，就往里灌数据就行。

```C

void TEST_Handler(char *pcInsert)
{
  //准备添加到shtml中的数据
    *pcInsert       = 'F';
    *(pcInsert + 1) = 'U';
    *(pcInsert + 2) = 'C';
    *(pcInsert + 3) = 'K';
}

```

至此，SSI 和 CGI 已经配置完成。

对keil工程进行编译即可。



在上一篇文章[webserver](https://www.yujincheng.net/2018/08/03/WebserverOnSTM32.html)中，我在OpenWRT上实现了网页。但是如果真正通过网页配置单片机各项功能，还需要把内存中的值写写到片上的FLASH中。并且能够把内存中的值读读取到内存中。

读写flash会用到额外的两个文件，stmflash.c 和 stmflsh.h，这两个文件加入了更新版本的工程项目中。

```Git
git clone https://git.coding.net/yujincheng/STM32F7_UCOS_LWIP.git
cd STM32F7_UCOS_LWIP
git checkout e73411eb662590b43877df7324c77b22bb8ffbc3
```

可以看到，在 HARDWARE/STMFLASH 文件夹下，有这两个文件。项目中应该已经包含了这两个文件，包括源文件和头文件。

如果没有包含头文件，需要再项目设置中，添加头文件。打开keil，选择 flash -> Config flash tools。找到C/C++ 选项卡，在include path 中，添加 ..\HARDWARE\STMFLASH 路径。

如下图所示

![配置include](https://www.yujincheng.net/post_img/2018-08-06/conf_include.jpg)

在需要读写flash的文件处，```#include "stmflash.h"```

若需要写入/读取 flash，调用函数，

```C
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);
void STMFLASH_Read(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);
```

三个参数依次为 flash 地址、写入/读取的内存数据指针、长度数目（多少个32bit）。

因为flash读写限制一定需要 4Byte 对齐。

flash 为下表。一般程序从 0x0800_0000 开始烧写，所以用户对flash操作的地址应该在程序段之后。在操作中，选择flash中最后一个主存储扇区，起始地址为 0x080C_0000。

| 块             | AXIM接口上的块基址        | ICTM接口上的块基址        | 大小  |
| -------------- | ------------------------- | ------------------------- | ----- |
| 主存储器 扇区0 | 0X0800 0000-0X0800   7FFF | 0X0020 0000-0X0020   7FFF | 32KB  |
| 主存储器 扇区1 | 0X0800 8000-0X0800   FFFF | 0X0020 8000-0X0020   FFFF | 32KB  |
| 主存储器 扇区2 | 0X0801 0000-0X0801   7FFF | 0X0021 0000-0X0021   7FFF | 32KB  |
| 主存储器 扇区3 | 0X0801 8000-0X0801   FFFF | 0X0021 8000-0X0021   FFFF | 32KB  |
| 主存储器 扇区4 | 0X0802 0000-0X0803   FFFF | 0X0022 0000-0X0023   FFFF | 128KB |
| 主存储器 扇区5 | 0X0804 0000-0X0807   FFFF | 0X0024 0000-0X0027   FFFF | 256KB |
| 主存储器 扇区6 | 0X0808 0000-0X080B   FFFF | 0X0028 0000-0X002B   FFFF | 256KB |
| 主存储器 扇区7 | 0X080C 0000-0X080F   FFFF | 0X002C 0000-0X002F   FFFF | 256KB |
| 系统存储器     | 0X1FF0 0000-0X1FF0   EDBF | 0X0010 0000-0X0010   EDBF | 60KB  |
| OTP区域        | 0X1FF0 F000-0X1FF0   F41F | 0X0010 F000-0X0010   F41F | 1056B |
| 选项字节       | 0X1FFF 0000-0X1FFF   001F |                           | 32B   |

在文件 LWIP\lwip_app\web_server_demo\httpd_cgi_ssi.c 中 154 行对文件进行写操作，即在网页配置了IP地址之后把IP地址从内存中写入flash中。为如下程序段的 13 行。其中FLASH_SAVE_ADDR 在 stmflash.h 中用宏定义设置为 ```#define FLASH_SAVE_ADDR 0x080C0000```，IP_addr_cache为缓存指针，长度为 5个4字节，共20bit。

```C
if (iIndex != -1)
    {
        for (i=0; i<iNumParams; i++)
        {
            if (strcmp(pcParam[i] , "IPaddr")==0)
          {
                for(ip_index = 0; ip_index < 20; ip_index++){
                    IP_addr_cache[ip_index] = pcValue[i][ip_index];
                    if(!IP_addr_cache[ip_index]){
                        break;
                    }
                }
                STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)IP_addr_cache,5);
          }
        }
     }
```

同样的，在文件 LWIP\lwip_app\web_server_demo\httpd_cgi_ssi.c 中 86 行。下列代码段的第4行。把数据从flash中读到内存中，再通过SSI显示到shtml上。

```C
for (ip_index = 0; ip_index < 20; ip_index++){
    IP_addr_cache[ip_index] = 0;
}
STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)IP_addr_cache,5);
for (ip_index = 0; ip_index < 20; ip_index++){
    *(pcInsert + ip_index) = IP_addr_cache[ip_index];
    if(!IP_addr_cache[ip_index]){
        break;
    }
}
```

相比于上一篇文章[webserver](https://www.yujincheng.net/2018/08/03/WebserverOnSTM32.html)中的实验，该实验也是在网页中配置一个字符串，然后显示到网页的另一个界面。所不同的是，该实验配置了flash，关机后可以保持上次配置的数据。