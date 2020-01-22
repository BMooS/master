# 前言

​		这学期学校开设了操作系统的课程，但是内容比较浅显基础，我认为操作系统作为程序员的基本功之一，比较重要，也就想自己多学点，就想用**c语言**写一个在**linux操作系统**上的**shell**，顺便复习一下大一学的c语言，也在用**《c和指针》**去复习，不得不说，这本书确实写的不错，当然，对初学者不是那么友好，有一定基础的人可以去看，很不错。

 		最后，这篇文章里的代码可以在[这里](<https://gitee.com/BMooS/myshell>)(gitee)或者[这里](https://github.com/BMooS/master)(github)看到。顺便说一下，作者再写这个程序时候只是一个在读学生，有些错误和粗浅之处，欢迎大家指正，谢谢大家。

# 如何实现shell 以及整体框架

​		首先让我们明白一个shell的**生命周期**，任何事物都有自己的生命周期，shell自然也不例外。

我们启动终端，终端就开始加载shell程序，

- **首先** shell会有自己的初始化，即加载并执行配置文件，这些配置会改变shell的行为

- **然后** shell程序启动，不断从标准输入中读取内容，并试图理解和执行这些内容

- **最后** 当所有命令完成后，控制shell程序关闭，并自动清除内存，自然退出

  

  

  *这样的程序描述自然过于简单，正常且普遍的shell的程序执行自然比这个复杂，但大体是这样的一个流程。*

  

  接下来我们简易去实现这个框架

  ```c
  int main(){
  	//配置文件
      
      //循环运行执行程序
      my_shell_loop();
      
      //shell程序退出
      
     	return EXIT_SUCCESS
   
  }
  ```

  

这里我们用循环去实现shell程序的主体部分，但shell程序不仅仅只有循环。

# 具体细节

## loop循环梗概

​		接下来让我们探讨loop循环的实现

​		shell程序在执行中不断**读取**标准输入中的内容，并加以**分析执行**，最后反馈给用户。

```c
int loop() {
    char *line;
    int state = 1;
    
    do{
        printf("myshell -> ");
        //读取标准输入中的内容，保存在line里面
        line = shell_readline();
        //分析并加以执行
        state = execute_line(line);
    }while(state);
    
    return 1;
}
```

## 命令读入

​		这里我们自己写入一个readline函数，具体分析程序是如何运作的。

**注意** 这里我们引入了动态内存，因为我们不可能限制并给定用户的输入长度，用户输入的字符串长度是未知的，所以这里使用动态内存来储存字符串。

```c
char *shell_readline() {
    int bufsize = 1024; //初始给定1024字符的长度
    int i = 0;
    char *buffer = malloc(sizeof(char)*bufsize);//缓存区里开辟bufsize大小的内存
    int c;

    if(!buffer){ //检查返回值
        printf("allocation error\n");
        exit(1);
    }
    while (1)
    {
        c = getchar();
        if(c == EOF || c == '\n'){
            buffer[i] = '\0';
            return buffer;
        }else{
            buffer[i] = c;
            i++;
        }
        if(i >= bufsize){ //当现有字符串数量大于bufsize时，重新分配2倍大小的内存空间
            bufsize += bufsize;
            buffer = realloc(buffer, sizeof(char)*bufsize);
            if(!buffer){ //检查返回指针
                printf("allocation error\n");
                exit(1);
            }
        }
    }
}
```

**注意** 在使用动态内存的时候一定要**小心**以下几点

- 调用malloc函数和realloc函数的时候要检查返回值，因为当申请新内存而现有资源不够的时候，就会返回NULL, 这是一个“null pointer”（空指针）
- 当然调用realloc函数时候，传递参值的时候也要小心，不要传递一个NULL，也不要传递一个非动态内存返回的指针
- 最后，**一定一定**注意内存的回收，即调用free()函数，回收动态内存空间，free函数的参值也是一个动态内存返回的指针，如果不回收动态内存，编译器不会报错，但是会造成**内存泄漏**，这是要极力避免的事情

这样就简易实现了一个readline函数，简单且实用，但是我们想要实现bash，zsh这些著名shell里，可以使用上下键调用历史命令，可以用tab键补全命令这些功能就有些无力了，好在我们有现成的轮子可以套用，那就是readline库。

```c
#include <readline/history.h>
line = readline("Myshell -> ");
if (!line) {
    printf("allocation error\n");
    exit(1);
}
add_history(line);
free(line);
```

**注意：** 

- readline库是一个**动态连接库**，gcc编译时候需要动态链接，使用一下指令即可： 

  ```
  gcc main.c -lreadline
  ```

- readline函数返回值是一个指针，指向一个动态内存区域，也需要判别是否为NULL以及使用free函数释放掉，防止内存泄漏

- 作者在这里只是很粗浅的使用，只是用了add_history()函数用于添加历史指令，readline函数有更高阶的操作，具体在这里[官方文档](<https://tiswww.case.edu/php/chet/readline/readline.html>)(注：需要一点科学上网手段)

  

  在这里，作者发现原始的颜色太单调，我们可以调制一些颜色对于shell提示输入符：

  ```c
  #define CLOSE "\001\033[0m\002"                 // 关闭所有属性
  #define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
  #define BEGIN(x,y) "\001\033["#x";"#y"m\002"	// x: 背景，y: 前景
  
   line = readline(BEGIN(49, 34)"Myshell->  "CLOSE);//使用一点宏定义简化一些复杂性
  ```

效果如下：

![UTOOLS1576411800363.png](https://i.loli.net/2019/12/15/78nbjUHFfsIBSQZ.png)

这样就好看多了

## 命令解析

​		现在我们来构造**execute_line()**函数。

​		 对于在shell里面输入一个命令，不仅有**内部命令**，即写入程序代码中的命令，和**外部命令**，即shell要fork一个新进程，进程去系统path中寻找可执行的程序去解析命令。还有对一个命令语句来说，有**命令部分**，还有**参数部分**，而我们要做的就是将一条命令语句中的参数和命令分开并处理。

​		即：

​				我们要在**execute_line()**构造**cut_line()**和**execute()**两个函数去实现。

### cut_line()

~~~ c
char **cut_line(char *line){
    int bufsize = 64, i = 0;
    char **tokens = malloc(bufsize*sizeof(char*));
    char *token;

    if(!tokens){
        printf("allocation error\n");
        exit(1);
    } 
    token = strtok(line, " ");
    while (token != NULL)
    {
        tokens[i] = token;
        i++;
        if(i >= bufsize){
            bufsize += bufsize;
            tokens = realloc(tokens, bufsize*sizeof(char*));
            if(!tokens){
                printf("allocation error\n");
                exit(1);
            }
        }
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;
    return tokens;
}
~~~

​		在这里我们先申请了一个字符指针的指针，大小为64个单位，用来保存一个字符串数组。

​		在cut_line()中我们用C 标准库**<string.h>**中**strtok()**函数：

> C 库函数
>
> **char \*strtok(char \*str, const char \*delim)**
>
>  分解字符串 **str** 为一组字符串，**delim** 为分隔符。
>
> 该函数返回被分解的第一个子字符串，如果没有可检索的字符串，则返回一个空指针。

**实例**

~~~c
#include <string.h>
#include <stdio.h>
 
int main () {
   char str[80] = "This is - BMooS - shell";
   const char s[2] = "-";
   char *token;
   
   /* 获取第一个子字符串 */
   token = strtok(str, s);
   
   /* 继续获取其他的子字符串 */
   while( token != NULL ) {
      printf( "%s\n", token );
    
      token = strtok(NULL, s);
   }
   
   return(0);
}
~~~

运行结果：

> This is
>
> BMooS
>
> shell

**注意：**

1. 还是和上述一样，在动态分配内存的的时候注意是否分配成功以及分配的多少，这里我采用的是几何倍数增长大小需求。
2. 在生成字符串数组的时候，记住在末尾加入空指针**NULL**。
3. cut_line()函数返回的是一个指向动态内存空间的指针，主要在外部调用的时候配合free()函数使用。

### execute()

​		execute()函数接受上面cut_line()函数的**返回值**，即execute()函数的参数是一个字符串数组，从变量类型上说，就是字符指针的指针。

​		这里我们在构造的时候要想到**对命令的提取**，**对命令的识别**以及**对命令的执行**。我们上述说到，对一个命令来说，我们识别它是一个内部命令还是一个外部命令，以便对命令**区分执行**。

~~~c
int execute(char **char_list){
    int i;
    if(char_list[0] == NULL){
        return 1;
    }
    for ( i = 0; i < long_cmd(); i++){
        if (strcmp(char_list[0], cmder[i]) == 0){
            return (*funcs[i])(char_list); 
        }
    }
    return process(char_list);//调用进程
}
~~~

**逻辑结构**

​		这里我们在构造execute()函数的时候我们对**char_list[0]**进行判空，不为空即有命令，我们要遍历我们在shell程序中编写的**内部命令函数**，看输入命令是否于其中内部命令匹配，若不匹配，则为外部命令，这时我们要调用我们编写的**process()**单独执行。



**内部命令**

​		我们这里需要构造一个字符串数组和一个自定义函数以及一个转换表：

1. 内部命令列表

~~~c
char *cmder[] = {
    "cd",
    "pwd",
    "help",
    "exit",
    "echo"
};
~~~

2. 返回内部命令列表长度的整型函数

~~~c
int long_cmd(){
    return sizeof(cmder)/sizeof(char*);
}
~~~

3. 转换表—函数指针数组

~~~c	
int (*funcs[])(char**) = {
    &fun_cd,
    &fun_pwd,
    &fun_help,
    &fun_exit,
    &fun_echo
};
~~~

​		**内部命令是编写在shell程序里面的，是自定义的，我们对它们起名也是自定义的。**

​		**注意：函数声明要在转换表之前**

现在让我们开始编写内部命令：

1. **cd**命令

~~~c
int fun_cd(char** char_list){
    if(char_list[1] == NULL){
        printf("Please enter the correct directory\n");
    }else{
        if(chdir(char_list[1]) != 0){
            perror("myshell");
        }
    }
    return 1;
}
~~~

使用系统函数**chdir()**改变当前工作目录  

函数说明：

1. 用户将当前的工作目录改变成以参数路径所指的目录。
2. 使用头文件 unistd.h。
3. chdir()函数返回值执行成功则返回0，失败返回-1，errno为错误代码。



2. **pwd**命令

~~~c
int fun_pwd(char** char_list){
    int bufsize = 1024;
    char *buffer = malloc(sizeof(char)*bufsize);
    if (!buffer){
        printf("allocation error1\n");
        exit(1);
    }
    while (1){
        if(getcwd(buffer, bufsize) == NULL){
            bufsize += bufsize;
            buffer = realloc(buffer, sizeof(char)*bufsize);
            if (!buffer){
                printf("allocation error\n");
                exit(1);
                }
        }else{
            printf("current working directory : %s\n", buffer);
            free(buffer);
            return 1;
        }
    }
}
~~~

函数使用getcwd()获得当前工作目录的绝对路径。

函数声明：

~~~c
#include<unistd.h>
char *getcwd(char *buf,size_t size);
~~~



函数说明：

1. getcwd()会将当前工作目录的绝对路径复制到参数buf所指的内存空间中,参数size为buf的空间大小。
2. 如果路径长度大于size,则会返回NULL。



3. **help**命令

~~~c
int fun_help(char** char_list){
    int i;
    printf("---------------------myshell---------------------\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("-----------The following are built in:-----------\n");
    for ( i = 0; i < long_cmd(); i++){
        printf("%s\n", cmder[i]);
    }
    printf("Use the man command for information on other programs.\n");
    printf("-------Support for simple pipes and redirects---------\n");
    printf("------------------------------------------------------\n");
    return 1;  
}
~~~

shell程序里调用help命令获得内部命令集。

4. **exit**命令

~~~c
int fun_exit(char** char_list){
    printf("-----------------------goodbye-----------------------\n");
    return 0;
}
~~~

exit命令返回值为0，可以中断loop循环，结束shell程序。

5. **echo**命令

~~~c
int fun_echo(char** char_list){
    int i;
    if (char_list[1] == NULL){
        printf("Enter correct output.\n");
    }else{
        for ( i = 1; char_list[i] != NULL; i++)
        {
            printf("%s ", char_list[i] );
        }
        printf("\n");
    }
    return 1;
}
~~~

echo命令用来输出后缀参数。

**注意：内部命令是自定义的，可扩充的。**



**外部命令**

​		这里我们要让原本shell程序执行的进程fork出两个进程，一个是**父进程**，一个为**子进程**。父进程是原本shell进程，阻塞并等待子进程执行结束。子进程用来执行外部命令。

~~~c
int process(char** char_list){
    pid_t pid = fork(),wpid;
    int status;
    if (pid == 0){
        if (execvp(char_list[0], char_list) == -1){
            perror("myshell ");
            exit(1);//子进程报错后销毁，返回父进程
        }
    }else if (pid < 0){
        perror("myshell ");
        exit(1);
    }else{
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}
~~~

上面使用到了三个系统函数，fork()，execvp()，waitpid()

关于fork函数，我之前写过一篇博客用来介绍，详细可以看[这里](https://bmoos.github.io/2020/01/15/fork/)。

关于execvp函数：

~~~c
int execvp(const char* file, const char* argv[]);
~~~

 第一个参数是要运行的文件，会在环境变量PATH中查找file并执行。

 第二个参数，是一个参数列表。

> execvp函数执行失败的时候，子进程是无法正常退出的，需要用exit强制退出该子进程，所以这时候就需要加个判断，当execvp执行失败返回-1时，调用exit()来退出子进程，不然该进程还是在那里，导致后边的shell程序无法正常执行。

execvp()是exec函数族里面其中之一，关于exec函数族，我之后会在写一篇博客用来介绍。

关于waitpid函数：

~~~c
pid_t waitpid(pid_t pid,int *status,int options)
~~~



在[这里](https://bmoos.github.io/2020/01/15/fork/)有wait函数用法，从本质上讲，系统调用waitpid和wait的作用是完全相同的，**但waitpid多出了两个可由用户控制的参数pid和options**，从而为我们编程提供了另一种更灵活的方式。

> 从参数的名字pid和类型pid_t中就可以看出，这里需要的是一个进程ID。但当pid取不同的值时，在这里有不同的意义。
>
> 1. pid>0时，只等待进程ID等于pid的子进程，不管其它已经有多少子进程运行结束退出了，只要指定的子进程还没有结束，waitpid就会一直等下去。
> 2. pid=-1时，等待任何一个子进程退出，没有任何限制，此时waitpid和wait的作用一模一样。
> 3. pid=0时，等待**同一个进程组**中的任何子进程，如果子进程已经加入了别的进程组，waitpid不会对它做任何理睬。
> 4. pid<-1时，等待一个**指定进程组**中的任何子进程，这个进程组的ID等于pid的绝对值。

> **options**提供了一些额外的选项来控制waitpid，目前在Linux中只支持**WNOHANG**和**WUNTRACED**两个选项，这是两个常数，可以用"|"运算符把它们连接起来使用 。

​	在关于父进程等待的时候，要注意子进程状态。

~~~c
do{
    wpid = waitpid(pid, &status, WUNTRACED);
}while (!WIFEXITED(status) && !WIFSIGNALED(status));
~~~

使用do......while结构，判断条件为

​				`!WIFEXITED(status) && !WIFSIGNALED(status)`

WIFEXITED(status) 这个宏用来指出子进程是否为正常退出的，如果是，它会返回一个非零值。

WIFSIGNALED(status)若子进程返回的状态为异常结束,则为真。

则对于父进程来说，子进程无论正常或者异常退出，循环语句都会跳出。

## 管道(匿名管道)

​		上述对shell程序的构建已经可以组成一个简单的shell程序了，有了命令的读入，命令的分析，以及命令的执行。但是对于一个成熟的shell，比如bash，zsh等，都会有管道功能，现在让我们实现管道功能。

​		**什么是管道：**

> Shell的一种功能，就是可以将两个或者多个命令（程序或者进程）连接到一起，把一个命令的输出作为下一个命令的输入，以这种方式连接的两个或者多个命令就形成了**管道（pipe）**。

![1Cjs5d.png](https://s2.ax1x.com/2020/01/19/1Cjs5d.png)

Linux 管道使用竖线 | 连接多个命令，这被称为管道符。Linux 管道的具体语法格式如下：

~~~c
command1 | command2
command1 | command2 | commandN... 
~~~

当在两个命令之间设置管道时，管道符 | 左边命令的输出就变成了右边命令的输入。只要第一个命令向标准输出写入，而第二个命令是从标准输入读取，那么这两个命令就可以形成一个管道。大部分的 Linux 命令都可以用来形成管道。

> 这里需要注意，command1 必须有正确输出，而 command2 必须可以处理 command2 的输出结果；而且 command2 只能处理 command1 的正确输出结果，不能处理 command1 的错误信息。

**管道机制：**

在Linux中，管道是一种使用非常频繁的通信机制。从本质上说，管道也是一种文件，但它又和一般的文件有所不同，管道可以克服使用文件进行通信的两个问题，具体表现为：

1. 限制管道的大小。实际上，管道是一个固定大小的缓冲区。在Linux中，该缓冲区的大小为1页，即4K字节，使得它的大小不象文件那样不加检验地增长。使用单个固定缓冲区也会带来问题，比如在写管道时可能变满，当这种情况发生时，随后对管道的write()调用将默认地被阻塞，等待某些数据被读取，以便腾出足够的空间供write()调用写。
2. 读取进程也可能工作得比写进程快。当所有当前进程数据已被读取时，管道变空。当这种情况发生时，一个随后的read()调用将默认地被阻塞，等待某些数据被写入，这解决了read()调用返回文件结束的问题。

注意：从管道读数据是一次性操作，数据一旦被读，它就从管道中被抛弃，释放空间以便写更多的数据。

**管道的实现：**

注意：我只实现了两条命令的管道机制，但是可以通过递归实现n条命令的管道，那样比较繁琐和抽象。

首先，让我们要对命令的读取加上对管道的识别：

~~~c
for (int i = 0; i < strlen(line); i++){
        if (line[i] == '|' && line[i+1] == ' ' && line[i-1] == ' '){
            sample = commandwithpipe(line);
            return sample;
        }
    }
~~~

其中commandwithpipe()是用来执行管道命令的：

~~~c
int commandwithpipe(char *line){
    int pipeIdx;
    for (int i = 0; i < strlen(line); ++i) {
        if (line[i] == '|' && line[i+1] == ' ' && line[i-1] == ' ') {
            pipeIdx = i;
            break;
        }
    }
    if (pipeIdx+2 == strlen(line)) { // 管道命令' | '后续没有指令，参数缺失
        printf("Parameters are missing\n");
        return 1;
    }

    int fds[2];
    if (pipe(fds) == -1) {
        return 0;
    }
    int result = 0;
    pid_t pid = fork();
    if (pid == -1) {
        result = 0;
    } else if (pid == 0) { // 子进程执行单个命令
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO); // 将标准输出重定向到fds[1]
        close(fds[1]);
        char *new_str = cut_str(0,pipeIdx-2,line);
        char **simple_line = cut_line(new_str);
        if (execute(simple_line) != 1){
            result = 1;
        }
        free(new_str);
        free(simple_line);
        exit(result);
    } else { // 父进程递归执行后续命令
        int status;
        waitpid(pid, &status, 0);

        close(fds[1]);
        dup2(fds[0], STDIN_FILENO); // 将标准输入重定向到fds[0]
        close(fds[0]);
        char *new_str = cut_str(pipeIdx + 2,strlen(line),line);
        char **simple_line = cut_line(new_str);
        result = execute(simple_line);
        free(new_str);
        free(simple_line);
    }

    return result;

}

~~~

commandwithpipe()函数中调用的cut_str()是将管道命令里面的两个命令切割下来：

~~~c
char *cut_str(int left,int right,char *line){
    int bufsize = 512;
    char *buffer = malloc(sizeof(char)*bufsize);
    int j = 0;
    if(!buffer){
        printf("allocation error7\n");
        exit(1);
    }
    for (size_t i = left; i <= right; i++){
        buffer[j] = line[i];
        j++;
    }
    buffer[j] = '\0';
    return buffer;
}
~~~

创建管道主要用到pipe函数，pipe的原型如下：

~~~c
#include <unistd.h>
int pipe(int fds[2]);
~~~

参数：一个整型数组，管道创建成功后，**fds[0]表示管道的读端，fds[1]表示管道的写端**。

成功返回0，失败返回-1。

如何用管道来实现进程间通讯，我们可以用以下的例子来实现以下：

~~~c
#include<stdio.h>
#include<unistd.h>
int main(){
    int fds[2];
    //1.创建管道
    if(pipe(fds)==-1){
        perror("pipe");
        return 1;
    }
    //2.fork子进程
    pid_t pid = fork();
    if(pid > 0){        //father
        //3.父进程关闭读端,向写端写入数据
        close(fds[0]);
        char buff[1024]={0};
        printf("parent to child#");
        fflush(stdout); //清空标准输出缓冲区
        ssize_t s = read(0,buff,sizeof(buff)-1);
        buff[s-1] = 0;
        write(fds[1],buff,s);
        close(fds[1]);
    }else if(pid==0){   //child
        //3.子进程关闭写端,从读端读出数据
        close(fds[1]);
        char buff[1024]={0};
        ssize_t s = read(fds[0],buff,sizeof(buff));
        printf("child to receive#%s\n",buff);
        close(fds[0]);
    }else{
        perror("fork");
        return 2;
    }
    wait(NULL); //回收子进程
    return 0;
}
~~~

注意：read和write函数的一个参数，是一个无符号整数，是**文件描述符**，用来表示一个文件。在Linux系统中，一切设备都看作文件。而每打开一个文件，就有一个代表该打开文件的文件描述符。程序启动时默认打开三个I/O设备文件：标准输入文件stdin，标准输出文件stdout，标准错误输出文件stderr，分别得到文件描述符 0, 1, 2。

**上述程序的功能是，父进程从标准输入读入，并且从管道写端fds[1]写入到管道中，子进程从管道读端fds[0]读出数据，并且输出到标准输出中，默认为屏幕。**

注意：上述程序并没有对父子进程的先后顺序做以处理，不过管道会自然实现，因为当管道中没有数据时，读取管道的进程，也就是父进程会被阻塞，等待管道中数据的写入。同时管道类似于通信中半双工信道的进程通信机制，一个管道可以实现双向的数据传输，而同一个时刻只能最多有一个方向的传输，不能两个方向同时进行。

现在在明白管道的原理，功能，以及使用后，让我们回头看看commandwithpipe()函数。

在函数中，我们对管道命令做了处理，使用fork分开执行，首先让子进程的标准输出重定向为管道写端fds[1]，然后使用execute()执行第一条命令语句。对于父程序，我们在等待子进程执行完后，先将标准输入重定向为管道读端fds[0]，然后用execute()执行第二条命令语句。

注意：我们在父进程中将标准输入进行了重定位，所以在执行完毕后要将其重定回来。

~~~c
int s_fd_out = dup(STDOUT_FILENO); //保存标准输出
int s_fd_in = dup(STDIN_FILENO);//保存标准输入

int n_fd_out = dup2(s_fd_out , STDOUT_FILENO);//恢复标准输出
int n_fd_in = dup2(s_fd_in,STDIN_FILENO);//恢复标准输入
~~~

我们在上述中用到了dup()以及dup2()函数，让我们来介绍一下：

~~~c
#include <unistd.h>
int dup(int oldfd);
int dup2(int oldfd, int newfd);
~~~

​		当调用dup函数时，内核在进程中创建一个新的文件描述符，此描述符是当前可用文件描述符的最小数值，这个文件描述符指向oldfd所拥有的文件表项。
　　dup2和dup的区别就是可以用newfd参数指定新描述符的数值，如果newfd已经打开，则先将其关闭。如果newfd等于oldfd，则dup2返回newfd, 而不关闭它。dup2函数返回的新文件描述符同样与参数oldfd共享同一文件表项。

现在我们构造好了一个简易的管道功能：

![1iUa0s.png](https://s2.ax1x.com/2020/01/20/1iUa0s.png)

## 输出重定向

一般情况下，每个 Linux 命令运行时都会打开三个文件：

- 标准输入文件(stdin)：stdin的文件描述符为0，Linux程序默认从stdin读取数据。
- 标准输出文件(stdout)：stdout 的文件描述符为1，Linux程序默认向stdout输出数据。
- 标准错误文件(stderr)：stderr的文件描述符为2，Linux程序会向stderr流中写入错误信息。

默认情况下，command > file 将 stdout 重定向到 file，command < file 将stdin 重定向到 file。

在我们程序中，鉴于我的技术有限，仅能实现输出重定向'>'。

首先我们得有对输出重定向命令的识别，它将加载在execute_line()函数中。

~~~c
for (int j = 0; j < strlen(line); j++){
	if (line[j] == '>'){
     	sample = commandWithRedi(line);
  	    return sample;
        }
}
~~~

然后对commandWithRedi()函数进行实现：

~~~c
int commandWithRedi(char* line) { //可能含有重定向
    int outNum = 0;
    char *outFile = NULL;
    int endIdx = strlen(line); // 指令在重定向前的终止下标

    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == '>') { // 输出重定向
            outNum++;
            if (i+1 < strlen(line))
                outFile = &line[i+1];
            else{
                printf("Parameters are missing\n");
            }
            endIdx = i;
        }
    }
    /* 处理重定向 */
    if (outNum > 1) { // 输出重定向符超过一个
        printf("Output redirection more than one\n");
        return 1;
    }

    int result = 1;
    pid_t pid = fork();
    if (pid == -1) {
        result = 0;
    } else if (pid == 0) {
        /* 输入输出重定向 */
        if (outNum == 1){
            freopen(outFile, "w", stdout);
        }
        /* 执行命令 */
        line[endIdx] = '\0';
        char** char_list = cut_line(line);
        int stute = execvp(char_list[0], char_list);
        free(char_list);
        if (stute == -1){
            exit(1);//子进程报错后销毁，返回父进程
        };
        exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);
        int err = WEXITSTATUS(status); // 读取子进程的返回码

        if (err) { 
            printf("Error: %s\n", strerror(err));
        }
    }
    return result;
}

~~~

在函数中我们用字符指针onFile对输出重定向的文件名进行标记，同时用到endIdx作为哨兵，记录重定向符>的位置，并在该位置上赋值'\0'，对执行命令的处理，用到freopen对标准输入stdout以写的方式重定向到onFile处。

实现：

![1EEFbD.png](https://s2.ax1x.com/2020/01/22/1EEFbD.png)

同时在1.txt文件中：

![1EE3Vg.png](https://s2.ax1x.com/2020/01/22/1EE3Vg.png)

则上述就简单实现了输出重定向功能。