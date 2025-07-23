#include "executor.h"


static int exec_command(t_ast_node *node) {
    if (!node->argv || !node->argv[0])
        return 0;
    pid_t pid = fork();
    if (pid == 0) {
        execvp(node->argv[0], node->argv);
        perror("execvp");
        _exit(127);
    }
    int status;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
}

static int exec_pipe(t_ast_node *node) {
    int fd[2];
    pipe(fd);
    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(fd[1], 1);
        close(fd[0]); close(fd[1]);
        exec_ast(node->left);
        _exit(0);
    }
    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(fd[0], 0);
        close(fd[0]); close(fd[1]);
        exec_ast(node->right);
        _exit(0);
    }
    close(fd[0]); close(fd[1]);
    waitpid(pid1, 0, 0);
    waitpid(pid2, 0, 0);
    return 0;
}
 
static int exec_redir(t_ast_node *node) {
    int file_fd = -1;
    int saved_fd = -1;
    int status = 1;

    if (node->redir_type == REDIR_IN) { 
		file_fd = open(node->redir_filename, O_RDONLY);
        if (file_fd < 0) {
            fprintf(stderr, "minishell: cannot open %s for reading\n", node->redir_filename);
            perror("open");
            return 1;
        }
        saved_fd = dup(0);         
        dup2(file_fd, 0);         
        close(file_fd);
        status = exec_ast(node->redir_cmd);
        if (saved_fd >= 0) {
            dup2(saved_fd, 0);     
            close(saved_fd);
        }
        return status;
    }

    if (node->redir_type == REDIR_OUT) { 
		file_fd = open(node->redir_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file_fd < 0) {
            fprintf(stderr, "minishell: cannot open %s for writing\n", node->redir_filename);
            perror("open");
            return 1;
        }
        saved_fd = dup(1);         
        dup2(file_fd, 1);          
        close(file_fd);
        status = exec_ast(node->redir_cmd);
        if (saved_fd >= 0) {
            dup2(saved_fd, 1);     
            close(saved_fd);
        }
        return status;
    }

    
    if (node->redir_type == REDIR_APPEND) {
        file_fd = open(node->redir_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (file_fd < 0) {
            fprintf(stderr, "minishell: cannot open %s for appending\n", node->redir_filename);
            perror("open");
            return 1;
        }
        saved_fd = dup(1);        
        dup2(file_fd, 1);         
        close(file_fd);
        status = exec_ast(node->redir_cmd);
        if (saved_fd >= 0) {
            dup2(saved_fd, 1);     
            close(saved_fd);
        }
        return status;
    }

    if (node->redir_type == REDIR_HEREDOC) {
        int pipe_fd[2];
        pid_t writer_pid;
        char *line = NULL;
        size_t bufsize = 0;
        ssize_t linelen;

        if (pipe(pipe_fd) < 0) {
            perror("pipe");
            return 1;
        }

        writer_pid = fork();
        if (writer_pid < 0) {
            perror("fork");
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            return 1;
        }

        if (writer_pid == 0) {
            while (1) {
                write(1, "> ", 2);
                linelen = getline(&line, &bufsize, stdin);
                if (linelen < 0)
                    break;
                if (linelen > 0 && line[linelen - 1] == '\n')
                    line[linelen - 1] = '\0';
                if (strcmp(line, node->redir_filename) == 0)
                    break;
                line[linelen - 1] = '\n';
                write(pipe_fd[1], line, linelen);
            }
            free(line);
            close(pipe_fd[1]);
            _exit(0);
        }

        waitpid(writer_pid, 0, 0);
        close(pipe_fd[1]);
        saved_fd = dup(0);          
		dup2(pipe_fd[0], 0);        
		close(pipe_fd[0]);
        status = exec_ast(node->redir_cmd);
        if (saved_fd >= 0) {
            dup2(saved_fd, 0);      
			close(saved_fd);
        }
        return status;
    }

    fprintf(stderr, "minishell: unknown redirection type\n"); 
	return 1;
}

int exec_ast(t_ast_node *node) {
    if (!node)
        return -1;
    if (node->type == 0)
	{
	//	if(is_builtin(node->argv[0])) 
		//	return exec_builtin(node->argv);
        return exec_command(node);
	}
    if (node->type == 1) // AST_PIPE
        return exec_pipe(node);
    if (node->type == 2) // AST_REDIR
        return exec_redir(node);
    return 0;
}
