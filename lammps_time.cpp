#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdlib.h>

using namespace std;

bool iscontain(char *line , char *key)
{
	if(strstr(line,key)!=NULL)
		return true;
	else
		return false;
}

int iskeyline(char *line)
{
	if(iscontain(line,"Memory usage per processor"))
		return 1;

	return 0;
}

void readcolumnname(char *line,vector<string> *cn)
{
	cn->clear();
	int start = 0;
	int end = 0;
	char key[100];
	bzero(key,100);
	while(true)
	{	
		if(line[start]==' ')
		{
			start++;
			end=start;
		}
		else
		{
			if(line[end]==' ')
			{
				memcpy(key,line+start,end-start);
				start=end;
				cn->push_back(string(key));
				bzero(key,100);
			}
			else
			{
				end++;
			}
		}

		if(start>=strlen(line))
			break;
	}
}	


void parsecolumnvalue(char *line,vector<double> *v)
{
        v->clear();
        int start = 0;
        int end = 0;
        char value[100];
        bzero(value,100);
        while(true)
        {
                if(line[start]==' ')
                {
                        start++;
                        end=start;
                }
                else
                {
                        if(line[end]==' ')
                        {
                                memcpy(value,line+start,end-start);
                                start=end;
                                v->push_back(atof(value));
                                bzero(value,100);
                        }
                        else
                        {
                                end++;
                        }
                }
                if(start>=strlen(line))
                        break;
        }
}


void readcolumnvalue(char *line,vector<string> *cn , vector< vector<double> > *cv)
{
	vector<double> value;
	parsecolumnvalue(line,&value);
	int i;
	for(i=0 ; i<cn->size() ; i++)
	{
		cv->at(i).push_back(value.at(i));
		//printf("cv->at(%d).push_back(%.2lf)\n",i,value.at(i));
	}

}

int state;
vector<string> columnname;
vector< vector<double> > columnvalue;
int totalstep;


vector<double> * findbycolumnname(char *name , vector<string> *cn , vector< vector<double> > *cv)
{
	int index;
	for(index=0 ; index<cn->size() ; index++)
	{
		if(strcmp(name,cn->at(index).c_str())==0)
		{
			return &(cv->at(index));
		}
	}
	return NULL;
}




int parseline(char *line)
{
	//printf("state=%d\n",state);
	int i;
	int nowstep;
	vector<double> * step;
	switch(state)
	{
		case 0 : //ignore head
		if(iskeyline(line)==1)
			state=1;

		break;
		case 1 : //read columns names
		readcolumnname(line,&columnname);
		//printf("columnnames:\n");
		for(i=0 ; i<columnname.size() ; i++)
		{
			//printf("%s\n",columnname.at(i).c_str());
			vector<double> values;
			values.clear();
			columnvalue.push_back(values);
		}	
		state=2;
		break;

		case 2:
		//printf("readcolumnvalue\n");
		step = findbycolumnname("Step",&columnname,&columnvalue);
		if(step!=NULL)
		{
			if(step->size()>0)
			{
				nowstep = step->at(step->size()-1);
			}
			//printf("step : %d/%d\n",nowstep,totalstep);
		}
		else
		{
			printf("column not found\n");
			exit(-1);
		}
		if(nowstep<totalstep)
		{
			readcolumnvalue(line,&columnname,&columnvalue);
		}
		break;

		default:
		break;
	}
}

int main(int argc , char **argv)
{
	totalstep=0;
	state=0;
	char line[1024];
	bzero(line,1024);
	int c=1;

	if(argc<2)
	{
		printf("./this <total step> < <log file>");
	}
	totalstep = atoi(argv[1]);

	printf("total step = %d\n",totalstep);
	

	while( gets(line) != NULL )
	{
		printf("<%d>%s\n",c++,line);
		parseline(line);
		bzero(line,1024);
	}
	
	state=3;

	for(int i=0 ; i<columnname.size() ; i++)
	{

		printf("%s:\n",columnname.at(i).c_str());
		for(int j=0 ; j<columnvalue.at(i).size() ; j++)
		{
			printf("%.2lf ",columnvalue.at(i).at(j));
		}
		printf("\n");
	}

	printf("result...\n");
	vector<double> *step =  findbycolumnname("Step",&columnname,&columnvalue);
	vector<double> *cpu =  findbycolumnname("CPU",&columnname,&columnvalue);
	if(step==NULL || cpu==NULL)
	{
		printf("CPU or Step column not found...\n");
		exit(-1);
	}

	double sumtime = cpu->at(cpu->size()-1) - cpu->at(0) ;
	double sumstep = step->at(step->size()-1) - step->at(0)  ;
	//printf("sumtime=%.2lf sumstep=%.2lf\n",sumtime,sumstep);
	double timeperstep = sumtime/sumstep;
	printf("Need %.2lf second\n",timeperstep*totalstep);



}
