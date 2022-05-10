#include "sched.h"
#include <stdlib.h>
#include <stdio.h>
void FCFS ( int number_of_jobs, const int job_submitted_time [],
			const int job_required_time [], int job_sched_start []){
	int i, tmp;
	job_sched_start[0] = job_submitted_time[0];
	for(i = 0;i < number_of_jobs - 1; i++){
		tmp = job_sched_start[i] + job_required_time[i];
		job_sched_start[i+1] = tmp < job_submitted_time[i+1] ? job_submitted_time[i+1] : tmp;
	}
}

struct Mist{
	int id;
	int time;
	struct Mist *next;
};
void SJF ( int number_of_jobs, const int job_submitted_time [],
           const int job_required_time [], int job_sched_start [] ){
	int i, time;
	struct Mist* head = NULL, *tmp,*p;
	head = malloc(sizeof(struct Mist));
	head->id = 0;
	head->time = job_required_time[0];
	head->next = NULL;
	for(i=1;i<number_of_jobs;i++){
		time = job_required_time[i];
		p =(struct Mist*)malloc(sizeof(struct Mist));
		p->id = i;
		p->time = time;
		for(tmp = head; tmp->next != NULL && time >= tmp->next->time; tmp = tmp->next);
		if(tmp == head){
			if(time >= head->time){
				head->next = p;
				p->next = NULL;
				continue;
			}
			p->next = head;
			head = p;
		} else{
			p->next = tmp->next;
			tmp->next = p;
			printf("he\n");
		}
	}
	p = head;
	job_sched_start[p->id] = job_submitted_time[0];
	for(i=0;i<number_of_jobs - 1 ;i++){
		time = job_sched_start[p->id] + job_required_time[p->id];
		p=p->next;
		job_sched_start[p->id] = time < job_submitted_time[p->id] ? job_submitted_time[p->id] : time;
	}
}
