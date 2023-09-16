#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "lib/pipes_fifo_seqnum.h"
#include "lib/error_functions.c"

int signal_came = 0;

void signal_handler()
{
	int saved = errno;
	signal_came = 1;
	errno = saved;
}

int pusher_signal_came = 0;
void pusher_signal_handler()
{
	int saved = errno;
	pusher_signal_came = 1;
	errno = saved;
}

int main(int argc, char const *argv[])
{
	if(argc != 3)
	{
		perror("Wrong number of inputs!");
		exit(EXIT_FAILURE);
	}

	if(strcmp(argv[1],"-i") != 0){
		perror("Wrong inputs!");
		exit(EXIT_FAILURE);	
	}

	int chef_ids[6];
	int fd;
	fd = open(argv[2], O_RDONLY);

	if(fd == -1)
	{
		perror("Input file couldn't be opened!");
		exit(EXIT_FAILURE);
	}

	int fd_wholesaler_gullac_counter = shm_open("/gullac_counter", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_wholesaler_gullac_counter == -1) errExit("shm_open");
	if (ftruncate(fd_wholesaler_gullac_counter, sizeof(int))) errExit("ftruncate");
	int * wholesaler_gullac_counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd_wholesaler_gullac_counter, 0);
	*wholesaler_gullac_counter = 0;
	close(fd_wholesaler_gullac_counter);
	
	int fd_wholesaler_semaphore = shm_open("/wholesaler_semaphore",O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_wholesaler_semaphore == -1) errExit("shm_open");
	if(ftruncate(fd_wholesaler_semaphore, sizeof(sem_t)) == -1) errExit("ftruncate");
	sem_t * wholesaler_semaphore = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd_wholesaler_semaphore, 0);
	sem_init(wholesaler_semaphore, 1, 0);
	close(fd_wholesaler_semaphore);

	int fd_delivered_ingredients = shm_open("/delivered_ingredients",O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_delivered_ingredients == -1) errExit("shm_open");
	if(ftruncate(fd_delivered_ingredients, sizeof(char) * 2) == -1) errExit("ftruncate");
	char * delivered_ingredients = mmap(NULL, sizeof(char) * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd_delivered_ingredients, 0);
	close(fd_delivered_ingredients);

	int fd_chef_semaphores = shm_open("/chef_semaphores",O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_chef_semaphores == -1) errExit("shm_open");
	if(ftruncate(fd_chef_semaphores, sizeof(sem_t) * 6) == -1) errExit("ftruncate");
	sem_t * chef_semaphores = mmap(NULL, sizeof(sem_t) * 6, PROT_READ | PROT_WRITE, MAP_SHARED, fd_chef_semaphores, 0);
	for (int i = 0; i < 6; ++i) sem_init(chef_semaphores + i, 1, 0);
	close(fd_chef_semaphores);	

	int fd_mutex_semaphore = shm_open("/mutex_semaphore",O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	ftruncate(fd_mutex_semaphore,sizeof(sem_t));
	sem_t * mutex_semaphore = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd_mutex_semaphore, 0);
	sem_init(mutex_semaphore, 1, 1);
	close(fd_mutex_semaphore);	

	int fd_chef_check = shm_open("/chef_check",O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_chef_check == -1) errExit("shm_open");
	if(ftruncate(fd_chef_check, sizeof(char) * 2) == -1) errExit("ftruncate");
	int * chef_check = mmap(NULL, sizeof(char) * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd_delivered_ingredients, 0);
	*chef_check = 0;
	close(fd_chef_check);

	for (int i = 0; i < 6; ++i)
	{		
		chef_ids[i] = fork();

		if(chef_ids[i] == 0)
		{
			struct sigaction sa;
			memset(&sa, 0, sizeof(sa));
			sa.sa_handler = &signal_handler;
			sa.sa_flags = 0;

			if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGUSR1,&sa,NULL) == -1))
			{
				perror("Failed to install signal sa_handler");
			}

		
		if(i == 0) printf("chef%d (pid %d) is waiting for Milk and Flour\n", i, getpid());
		else if(i == 1) printf("chef%d (pid %d) is waiting for Milk and Walnut\n", i, getpid());
		else if(i == 2) printf("chef%d (pid %d) is waiting for Milk and Sugar\n", i, getpid());
		else if(i == 3) printf("chef%d (pid %d) is waiting for Flour and Walnut\n", i, getpid());
		else if(i == 4) printf("chef%d (pid %d) is waiting for Flour and Sugar\n", i, getpid());
		else if(i == 5) printf("chef%d (pid %d) is waiting for Walnut and Sugar\n", i, getpid());

		if(*chef_check == 5)
		{
			sem_post(wholesaler_semaphore);
		}

		(*chef_check)++;
	
			int local_gullac_counter = 0;

			while(1)
			{
				if(signal_came)
				{				
					sem_wait(mutex_semaphore);
					printf("chef%d (pid %d) is exiting\n",i, getpid());
					sem_post(mutex_semaphore);
					return local_gullac_counter;
				}

				if(sem_wait(chef_semaphores + i) == -1) continue;
					
				if(i == 0)
				{
					sem_wait(mutex_semaphore);

						printf("chef%d (pid %d) has taken the Milk (%s)\n", i, getpid(), delivered_ingredients);						
						printf("chef%d (pid %d) has taken the Flour (%s)\n", i, getpid(), delivered_ingredients);						
						printf("chef%d (pid %d) is preparing the dessert (%s)\n", i, getpid(), delivered_ingredients);						
						local_gullac_counter++;

						delivered_ingredients[1] = 'X';
						delivered_ingredients[0] = 'X';

						sem_post(wholesaler_semaphore);
						printf("chef%d (pid %d) has delivered the dessert (%s)\n", i, getpid(), delivered_ingredients);
					

					sem_post(mutex_semaphore);
				}
				else if(i == 1)
				{
					sem_wait(mutex_semaphore);
				
						printf("chef%d (pid %d) has taken the Milk (%s)\n", i, getpid(), delivered_ingredients);						
						printf("chef%d (pid %d) has taken the Walnut (%s)\n", i, getpid(), delivered_ingredients);	
						printf("chef%d (pid %d) is preparing the dessert (%s)\n", i, getpid(), delivered_ingredients);
						local_gullac_counter++;

						delivered_ingredients[1] = 'X';
						delivered_ingredients[0] = 'X';

						sem_post(wholesaler_semaphore);
						printf("chef%d (pid %d) has delivered the dessert (%s)\n", i, getpid(), delivered_ingredients);
					

					sem_post(mutex_semaphore);
				}
				else if(i == 2)
				{

					sem_wait(mutex_semaphore);

					
						printf("chef%d (pid %d) has taken the Milk (%s)\n", i, getpid(), delivered_ingredients);						
						printf("chef%d (pid %d) has taken the Sugar (%s)\n", i, getpid(), delivered_ingredients);
						printf("chef%d (pid %d) is preparing the dessert (%s)\n", i, getpid(), delivered_ingredients);
						local_gullac_counter++;

						delivered_ingredients[1] = 'X';
						delivered_ingredients[0] = 'X';

						sem_post(wholesaler_semaphore);
						printf("chef%d (pid %d) has delivered the dessert (%s)\n", i, getpid(), delivered_ingredients);
					

					sem_post(mutex_semaphore);
				}
				else if(i == 3)
				{
					sem_wait(mutex_semaphore);

					
						printf("chef%d (pid %d) has taken the Flour (%s)\n", i, getpid(), delivered_ingredients);						
						printf("chef%d (pid %d) has taken the Walnut (%s)\n", i, getpid(), delivered_ingredients);	
						printf("chef%d (pid %d) is preparing the dessert (%s)\n", i, getpid(), delivered_ingredients);
						local_gullac_counter++;

						delivered_ingredients[1] = 'X';
						delivered_ingredients[0] = 'X';

						sem_post(wholesaler_semaphore);
						printf("chef%d (pid %d) has delivered the dessert (%s)\n", i, getpid(), delivered_ingredients);
					

					sem_post(mutex_semaphore);
				}
				else if(i == 4)
				{
					sem_wait(mutex_semaphore);

					
						printf("chef%d (pid %d) has taken the Flour (%s)\n", i, getpid(), delivered_ingredients);						
						printf("chef%d (pid %d) has taken the Sugar (%s)\n", i, getpid(), delivered_ingredients);	
						printf("chef%d (pid %d) is preparing the dessert (%s)\n", i, getpid(), delivered_ingredients);
						local_gullac_counter++;

						delivered_ingredients[1] = 'X';
						delivered_ingredients[0] = 'X';

						sem_post(wholesaler_semaphore);
						printf("chef%d (pid %d) has delivered the dessert (%s)\n", i, getpid(), delivered_ingredients);
					

					sem_post(mutex_semaphore);
				}
				else if(i == 5)
				{
					sem_wait(mutex_semaphore);

												
						printf("chef%d (pid %d) has taken the Walnut (%s)\n", i, getpid(), delivered_ingredients);						
						printf("chef%d (pid %d) has taken the Sugar (%s)\n", i, getpid(), delivered_ingredients);
						printf("chef%d (pid %d) is preparing the dessert (%s)\n", i, getpid(), delivered_ingredients);	
						local_gullac_counter++;

						delivered_ingredients[1] = 'X';
						delivered_ingredients[0] = 'X';

						sem_post(wholesaler_semaphore);
						printf("chef%d (pid %d) has delivered the dessert (%s)\n", i, getpid(), delivered_ingredients);
					

					sem_post(mutex_semaphore);
				}
				
			}

			exit(EXIT_SUCCESS);
		}
	}

	int fd_pusher_semaphores = shm_open("/pusher_semaphores",O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if(ftruncate(fd_pusher_semaphores,sizeof(sem_t) * 4) == -1) exit(EXIT_FAILURE);
	sem_t* pusher_semaphores = mmap(NULL, sizeof(sem_t) * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd_pusher_semaphores, 0);
	for (int i = 0; i < 4; ++i) sem_init(pusher_semaphores + i, 1, 0);	
	if(close(fd_pusher_semaphores) == -1) perror("Error occured while trying to close file!");
		

	int fd_checks = shm_open("/checks",O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	ftruncate(fd_checks,sizeof(int) * 4);
	int* checks = mmap(NULL, sizeof(int) * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd_checks, 0);
	for (int i = 0; i < 4; ++i) checks[i] = 0;
		
	int pusher_ids[4];

	for(int i = 0; i < 4; i++)
	{
		pusher_ids[i] = fork();
		if(pusher_ids[i] == 0)
		{
			struct sigaction sa;
			memset(&sa,0,sizeof(sa));
			sa.sa_handler = &pusher_signal_handler;
			sa.sa_flags = 0;
			if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGUSR1,&sa,NULL) == -1)) perror("Failed to install signal sa_handler");
		
			if(i == 0)
			{		
				while(1)
				{
					if(pusher_signal_came) exit(EXIT_SUCCESS);
						
					if(sem_wait(pusher_semaphores + 0) == -1) continue;
							
					sem_wait(mutex_semaphore);

					if(delivered_ingredients[0] != 'M' && delivered_ingredients[1] != 'M')
					{
						sem_post(mutex_semaphore);
						continue;
					}

					if(checks[1])
					{
						checks[1] = 0;
						sem_post(chef_semaphores + 0);
					}
					else if(checks[2])
					{
						checks[2] = 0;
						sem_post(chef_semaphores + 1);
					}
					else if(checks[3])
					{
						checks[3] = 0;
						sem_post(chef_semaphores + 2);
					}
					else
					{
						checks[0] = 1;	
					}
					sem_post(mutex_semaphore);
				}
			}
			else if(i == 1)	
			{
				while(1)
				{
					if(pusher_signal_came) exit(EXIT_SUCCESS);
						
					if(sem_wait(pusher_semaphores + 1) == -1) continue;
							
					sem_wait(mutex_semaphore);

					if(delivered_ingredients[0] != 'F' && delivered_ingredients[1] != 'F')
					{
						sem_post(mutex_semaphore);
						continue;
					}
					if(checks[0])
					{
						checks[0] = 0;
						sem_post(chef_semaphores + 0);
					}
					else if(checks[2])
					{
						checks[2] = 0;
						sem_post(chef_semaphores + 3);
					}
					else if(checks[3])
					{
						checks[3] = 0;
						sem_post(chef_semaphores + 4);
					}
					else
					{
						checks[1] = 1;	
					}
					sem_post(mutex_semaphore);
				}
			}
			else if(i == 2)	
			{
				while(1)
				{
					if(pusher_signal_came) exit(EXIT_SUCCESS);
						
					if(sem_wait(pusher_semaphores + 2) == -1) continue;
							
					sem_wait(mutex_semaphore);

					if(delivered_ingredients[0] != 'W' && delivered_ingredients[1] != 'W')
					{
						sem_post(mutex_semaphore);
						continue;
					}
					if(checks[0])
					{
						checks[0] = 0;
						sem_post(chef_semaphores + 1);
					}
					else if(checks[1])
					{
						checks[1] = 0;
						sem_post(chef_semaphores + 3);
					}
					else if(checks[3])
					{
						checks[3] = 0;
						sem_post(chef_semaphores + 5);
					}
					else
					{
						checks[2] = 1;	
					}
					sem_post(mutex_semaphore);
				}
			}
			else if(i == 3)	
			{
				while(1)
				{
					if(pusher_signal_came) exit(EXIT_SUCCESS);
						
					if(sem_wait(pusher_semaphores + 3) == -1) continue;
							
					sem_wait(mutex_semaphore);

					if(delivered_ingredients[0] != 'S' && delivered_ingredients[1] != 'S')
					{
						sem_post(mutex_semaphore);
						continue;
					}
					if(checks[0])
					{
						checks[0] = 0;
						sem_post(chef_semaphores + 2);
					}
					else if(checks[1])
					{
						checks[1] = 0;
						sem_post(chef_semaphores + 4);
					}
					else if(checks[2])
					{
						checks[2] = 0;
						sem_post(chef_semaphores + 5);
					}
					else
					{
						checks[3] = 1;	
					}
					sem_post(mutex_semaphore);
				}
			}

			exit(EXIT_SUCCESS);
		}
	}

	sem_wait(wholesaler_semaphore);
	size_t bytes_read;
	unsigned char buffer[3];
	int textLength = 0;
	do
	{	
		bytes_read = read(fd, buffer, sizeof(buffer));
		delivered_ingredients[0] = buffer[0];
		delivered_ingredients[1] = buffer[1];

		printf("\nthe wholesaler (pid %d) delivers ", getpid());

		if(delivered_ingredients[0] == 'M') printf("Milk ");
		else if(delivered_ingredients[0] == 'S') printf("Sugar ");
		else if(delivered_ingredients[0] == 'W') printf("Walnut ");
		else if(delivered_ingredients[0] == 'F') printf("Flour ");

		if(delivered_ingredients[1] == 'M') printf("Milk\n");
		else if(delivered_ingredients[1] == 'S') printf("Sugar\n");
		else if(delivered_ingredients[1] == 'W') printf("Walnut\n");
		else if(delivered_ingredients[1] == 'F') printf("Flour\n");

		printf("the wholesaler (pid %d) is waiting for the dessert\n", getpid());

		for (int i = 0; i < 4; ++i) sem_post(pusher_semaphores + i);	// Calls pushers
			
		textLength += bytes_read;

		sem_wait(wholesaler_semaphore);

		printf("the wholesaler (pid %d) has obtained the dessert and left\n", getpid());
		sleep(1);

	}
	while(bytes_read == sizeof(buffer));

	for (int i = 0; i < 4; ++i) kill(pusher_ids[i],SIGUSR1);
		
	for (int i = 0; i < 6; ++i) kill(chef_ids[i],SIGUSR1);
	
	int status;

	for (int i = 0; i < 4; ++i) waitpid(pusher_ids[i], &status, 0);	

	for (int i = 0; i < 6; ++i)
	{
		waitpid(chef_ids[i], &status, 0);
		int return_value = WEXITSTATUS(status);	
		*wholesaler_gullac_counter += return_value;
	} 
	
	printf("the wholesaler (pid %d) is done (total desserts: %d)\n", getpid(), *wholesaler_gullac_counter);

	if(close(fd) == -1) perror("Error occured while trying to close file after reading!");
	
	shm_unlink("/mutex_semaphore");
	shm_unlink("/wholesaler_gullac_counter");
	shm_unlink("/delivered_ingredients");
	shm_unlink("/chef_semaphores");
	shm_unlink("/wholesaler_semaphore");
	shm_unlink("/pusher_semaphores");
	shm_unlink("/checks");
	shm_unlink("/gullac_counter");
	return 0;
}