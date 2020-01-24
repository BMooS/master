#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h> //gcc main.c -lreadline

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景


char *cmder[] = {
    "cd",
    "pwd",
    "help",
    "exit",
    "echo"
};

int long_cmd(){
    return sizeof(cmder)/sizeof(char*);
}

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
                printf("allocation error2\n");
                exit(1);
                }
        }else{
            printf("current working directory : %s\n", buffer);
            free(buffer);
            return 1;
        }
    }
}

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

int fun_exit(char** char_list){
    printf("-----------------------goodbye-----------------------\n");
    return 0;
}

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

int (*funcs[])(char**) = {
    &fun_cd,
    &fun_pwd,
    &fun_help,
    &fun_exit,
    &fun_echo
};

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


char *shell_readline() {
    int bufsize = 1024;
    int i = 0;
    char *buffer = malloc(sizeof(char)*bufsize);
    int c;

    if(!buffer){
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
        if(i >= bufsize){
            bufsize += bufsize;
            buffer = realloc(buffer, sizeof(char)*bufsize);
            if(!buffer){
                printf("allocation error\n");
                exit(1);
            }
        }
    }
}

char **cut_line(char *line){
    int bufsize = 64, i = 0;
    char **tokens = malloc(bufsize*sizeof(char*));
    char *token;

    if(!tokens){
        printf("allocation error5\n");
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
                printf("allocation error6\n");
                exit(1);
            }
        }
        token = strtok(NULL, " ");
    }
    tokens[i] = NULL;
    return tokens;
}

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



int execute_line(char *line){
    int sample;
    for (int i = 0; i < strlen(line); i++){
        if (line[i] == '|' && line[i+1] == ' ' && line[i-1] == ' '){
            sample = commandwithpipe(line);
            return sample;
        }
    }
    for (int j = 0; j < strlen(line); j++){
        if (line[j] == '>'){
            sample = commandWithRedi(line);
            return sample;
        }
    }
    char **simple_line = cut_line(line);
    sample = execute(simple_line);
    free(simple_line);
    return sample;
}

int loop() {
    char *line;
    int state = 1;

    do{
        int s_fd_out = dup(STDOUT_FILENO);
        int s_fd_in = dup(STDIN_FILENO);
        //printf("myshell-> ");
        //line = shell_readline();
        line = readline(BEGIN(49, 34)"Myshell->  "CLOSE);
        if (!line){
            printf("allocation error\n");
            exit(1);
        }
        add_history(line);

        state = execute_line(line);
        
        free(line);
        int n_fd_out = dup2(s_fd_out , STDOUT_FILENO);
        int n_fd_in = dup2(s_fd_in,STDIN_FILENO);
    }while(state);
    return 1;
}

int main() {
    //预留配置文件位置
    loop();
    exit(0);
}