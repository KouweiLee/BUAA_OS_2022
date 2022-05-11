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
	job_sched_start[0] = job_submitted_time[0];
	int id, now = 1, i, time;
	int finish = job_sched_start[0] + job_required_time[0];
	struct Mist *head =NULL, *p=NULL, *tmp=NULL;
	while(now != number_of_jobs){
		if(head == NULL && finish < job_submitted_time[now]){
			finish = job_submitted_time[now];
		}
		for(i=now;finish>=job_submitted_time[i] && i < number_of_jobs;i++){
		//	LIST_INSERT(i);
			time = job_required_time[i];
			p = (struct Mist*) malloc(sizeof(struct Mist));
			p->id = i;
			p->time = time;
			p->next = NULL;
			if(head == NULL) head = p;
		    else {
				for(tmp=head;tmp->next!=NULL&&tmp->next->time <= time;tmp = tmp->next);
				if(tmp == head){
					if(head->time <= time){
						p->next = head->next;
						head->next = p;
					} else {
						p->next = head;
						head = p;
					}
				}else {
					p->next = tmp->next;
					tmp->next = p;
				}
			}
		}
		if(head != NULL){
			p = head;
			id = p->id;
			head = head->next;
			free(p);
			job_sched_start[id] = finish;
			finish = job_sched_start[id] + job_required_time[id];
			now = i;
		}
	}       	
	while(head!=NULL){
		p = head;
		id = p->id;
		head = head->next;
		free(p);
		job_sched_start[id] = finish;
		finish = job_sched_start[id] + job_required_time[id];
	}

}
