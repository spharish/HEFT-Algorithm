#include <bits/stdc++.h>
using namespace std;
typedef long long int LL;

LL n_p, n_t; //no of processors and tasks
LL tmp, a1, a2, a3; //temporary variables
vector < vector <LL> > comm_cost;
vector < vector <LL> > extra;
string dummy;
class task {
public:
	LL id;
	vector <double> eft;
	vector <double> est;
	vector <double> exec; //execution time
	vector <LL> par; //parent tasks
	double rank;
	bool flag; //for not finding the rank again and again in recursive function
	double avg;
	LL final_proc; //final processor
	double ft; //final finsih time
	double st; //final start time
};
class proc {
public:
	LL tasks_done;
	vector < pair<double, double> >  time;//stores finish times of the tasks executed in that proc	
};
vector <task> t, t1;
vector <proc> p;

void input_exec_time() {
	t.resize(n_t);
	t1.resize(n_t);
	extra.resize(n_t);
	for (LL i = 0; i < n_t; ++i)
	{
		t[i].avg = 0;
		t[i].exec.resize(n_p);
		t[i].eft.resize(n_p);
		t[i].est.resize(n_p);
		t[i].id = i;
		t1[i].id = i;
		extra[i].resize(n_p);
		cin >> dummy;
		for (LL j = 0; j < n_p; ++j)
		{
			cin >> tmp;
			t[i].exec[j] = tmp;
			t[i].avg += tmp;
			extra[i][j] = tmp;
		}

		t[i].avg = (double) t[i].avg / (double)n_p ;
	}
}
void input_comm_cost() {
	comm_cost.resize(n_t);
	for (LL j = 0; j < n_t; ++j) {
		comm_cost[j].resize(n_t);
		for (LL i = 0; i < n_t; ++i)
		{
			comm_cost[j][i] = -1;
		}
	}

	while (true) {
		cin >> dummy;
		cin >> a1;
		if (a1 == -1) {
			return ;
		}
		cin >> a2 >> a3;
		comm_cost[a1][a2] = a3;
		t[a2].par.push_back(a1); //parent
	}
}
void input() {
	//cout << "*";
	cin >> dummy >> n_p >> dummy >>  n_t;

	
	input_exec_time();
	for (LL i = 0; i < n_t; ++i)
	{
		t[i].flag = false;
	}
	input_comm_cost();


	p.resize(n_p);
	for (LL i = 0; i < n_p; ++i)
	{		
		p[i].tasks_done = 0;
	}
}
double find_rank(LL index) {
	if (t[index].flag == true) {
		return t[index].rank;
	}

	double maxi = 0.0;
	for (LL i = 0; i < comm_cost[index].size(); ++i)
	{
		if (comm_cost[index][i] != -1) {
			//cout << index << " " << i;
			maxi = max(maxi, comm_cost[index][i] + find_rank(i));
		}
	}

	t[index].rank = maxi + t[index].avg;
	t1[index].rank = t[index].rank;
	t[index].flag = true;
	return t[index].rank;
}
bool sort_by_rank(const task &t11, const task &t2) {
	if (t11.rank != t2.rank)
		return (t11.rank > t2.rank);
	else
		return (t11.id < t2.id); //if ranks are equal, then consider the lesser index
}
void print_by_rank() {
	for (LL i = 0; i < n_t; ++i)
	{
		cout << t1[i].id << " ";;
	}
	cout << endl;
}
void compute(LL index) {
	LL m_pi = 0;
	for (LL pi = 0; pi < n_p; ++pi)
	{
		double maxi = 1;
		for (LL i = 0; i < t[index].par.size(); ++i)
		{
			if (pi == t[t[index].par[i]].final_proc) {
				maxi = max(maxi, t[t[index].par[i]].ft + 1);
			}
			else {
				maxi = max(maxi, (comm_cost[t[index].par[i]][index] + t[t[index].par[i]].ft + 1));
			}
			/*if (index == 3 and pi == 1)
			cout << maxi << endl;*/
		}

		t[index].est[pi] = maxi;
		t[index].eft[pi] = maxi + t[index].exec[pi] - 1;
		/*if (index == 3 and pi == 1) {
			cout << t[index].exec[pi] ;
			cout << endl << t[index].eft[pi] <<"*" << endl;
		}*/
		//Computed initial est

		bool check = false;
		if (p[pi].tasks_done == 0) 
			check = true;
		if (p[pi].tasks_done > 0 and t[index].eft[pi] < p[pi].time[0].second) {
			check = true;
		}
		for (LL i = 0; i < p[pi].time.size()-1; ++i) //adjusting est
		{
			if (check == true)
				break;
			if (t[index].est[pi] > p[pi].time[i].first and t[index].eft[pi] < p[pi].time[i+1].second) {
				check = true;
			}
			else if (t[index].est[pi] <= p[pi].time[i].first and p[pi].time[i+1].second - p[pi].time[i].first - 1 >= t[index].exec[pi]) {
				check = true;
				t[index].est[pi] = p[pi].time[i].first + 1;
			}
		}

		if (check == false) {
			if (t[index].est[pi] <= p[pi].time[p[pi].tasks_done-1].first) {
				t[index].est[pi] = p[pi].time[p[pi].tasks_done-1].first + 1;
				check = true;
			}
		}
		//We have just computed adjusted est

		t[index].eft[pi] = t[index].est[pi] + t[index].exec[pi] - 1;
		if (t[index].eft[pi] < t[index].eft[m_pi]) {
			m_pi = pi;
		}
	}

	t[index].ft = t[index].eft[m_pi]; //found final finish time for the task;
	t[index].st = t[index].est[m_pi];
	t[index].final_proc = m_pi;
	p[m_pi].tasks_done++;

	
	p[m_pi].time.push_back(make_pair(t[index].ft, t[index].st));
	sort(p[m_pi].time.begin(), p[m_pi].time.end());
}

int main()
{
	//freopen("/home/spharish/input.txt", "r", stdin);
	//freopen("/home/spharish/SEMESTERS/Sem 4/PPL/HEFT/in-out/output.txt", "w", stdout);
	input();	

	for (LL i = 0; i < n_t; ++i) {
		t[i].rank = find_rank(t[i].id);
	}

	sort(t1.begin(), t1.end(), sort_by_rank);		
	
	//print_by_rank();

	for (LL i = 0; i < n_t; ++i)
	{
		compute(t1[i].id);
	}

	double ans = 0;
	for (LL i = 0; i < n_t; ++i)
	{
		ans = max(ans, t[i].ft);
		//cout << "Id:" << t[i].id << " Starting Time:" << t[i].st << " Finish Time:" << t[i].ft << " Processor Number: "  << t[i].final_proc << " Exec Time: " << t[i].exec[t[i].final_proc] << endl;
	}

	cout << ans << endl;
	for (LL i = 0; i < n_t; ++i)
	{
		for (LL j = 0; j < n_p; ++j)
		{
			if (j == t[i].final_proc)
				cout << "1" << endl;
			else 
				cout << "0" << endl;
		}
	}

	for (LL i = 0; i < n_t; ++i)
	{
		for (LL j = 0; j < n_p; ++j)
		{
			if (j == t[i].final_proc)
				cout << t[i].st-1<< endl;
			else 
				cout << "0" << endl;
		}
	}

	for (LL i = 0; i < n_t; ++i)
	{
		for (LL j = 0; j < n_p; ++j)
		{
			if (j == t[i].final_proc)
				cout << t[i].ft << endl;
			else 
				cout << "0" << endl;
		}
	}

	return 0;
}