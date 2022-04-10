#include <iostream>
#include "windows.h"
#include <vector>
#include <iterator>
#include <cmath>
#include <algorithm>

using namespace std;

void swap(int* a, int i, int j)
{
	int s = a[i];
	a[i] = a[j];
	a[j] = s;
}
bool NextSet(int* a, int n)
{
	int j = n - 2;
	while (j != -1 && a[j] >= a[j + 1]) j--;
	if (j == -1)
		return false; // больше перестановок нет
	int k = n - 1;
	while (a[j] >= a[k]) k--;
	swap(a, j, k);
	int l = j + 1, r = n - 1; // сортируем оставшуюся часть последовательности
	while (l < r)
		swap(a, l++, r--);
	return true;
}
vector<vector<int>> Swap(int n) {
	int* a;
	a = new int[n];
	vector<vector<int>> sw;
	vector<int> sw_begin;
	for (int i = 0; i < n; i++) {
		a[i] = i + 1;
		sw_begin.push_back(i);
	}
	sw.push_back(sw_begin);
	while (NextSet(a, n)) {
		vector<int> sw_str;
		for (int j = 0; j < n; j++) {
			sw_str.push_back(a[j] - 1);
		}
		sw.push_back(sw_str);
	}
	return sw;
}

class BOX {
private:
	int disks;
	vector<double> Box;
	vector<double> Box_min;
	vector<vector<double>> Box_min_n;
	vector<vector<double>> Box_memory{ {} };
	double Sum_min = 1000000;
	double x_i(double R2, vector<double> buf_box, vector<double> buf_X) {
		vector<double> X_R2;
		double x_r2 = 0;
		for (int i = 0; i < buf_X.size(); i++) {
			x_r2 = buf_X[i] + 2 * (sqrt(R2 * buf_box[i]));
			X_R2.push_back(x_r2);
		}
		x_r2 = 0;
		for (int i = X_R2.size() - 1; i >= 0; i--) {
			if (x_r2 < X_R2[i]) x_r2 = X_R2[i];
		}
		return x_r2;
	}
	double result_min(vector<double> buf_box, vector<double> buf_X) {
		vector<double> left, right;
		for (int i = 0; i < buf_X.size(); i++) {
			left.push_back(buf_X[i] - buf_box[i]);
			right.push_back(buf_X[i] + buf_box[i]);
		}
		double min_left, max_right;
		min_left = *min_element(left.begin(), left.end());
		max_right = *max_element(right.begin(), right.end());
		return (max_right - min_left);
	}
	double Sum_min_ij(vector<double> buf_box) { //2n^2 + nu
		vector<double> buf_X = { buf_box[0] };
		for (int i = 1; i < buf_box.size(); i++) {
			buf_X.push_back(x_i(buf_box[i], buf_box, buf_X));
		}
		return result_min(buf_box, buf_X);
	}

	void Sum_min_function() {
		vector<vector<int>> buf_box = Swap(Box.size());
		double buf_sum;
		for (int i = 0; i < buf_box.size(); i++) {
			vector<double> R_box;
			for (int j = 0; j < Box.size(); j++) {
				R_box.push_back(Box[buf_box[i][j]]);
			}
			buf_sum = Sum_min_ij(R_box);
			if (Sum_min > buf_sum) {
				Box_min_n.clear();
				Sum_min = buf_sum;
				Box_min = R_box;
			}
			if (Sum_min == buf_sum) {
				Box_min_n.push_back(R_box);
			}
		}
	}

	void Output_result() {
		for (int i = 0; i < Box_min_n.size(); i++) {
			cout << "BOX" << i << " =";
			for (int j = 0; j < Box_min_n[i].size(); j++) {
				cout << " " << Box_min_n[i][j];
			}
			cout << endl;
		}
		cout << "BOX =";
		for (int i = 0; i < Box_min.size(); i++) {
			cout << " " << Box_min[i];
		}
		cout << endl;
		cout << "Min(Sum) = " << Sum_min << endl;
	}
	vector<vector<double>> Benefit(vector<double> check) { // 3(1 number)(abb), 4(2 number)(abba); a - change
		vector<vector<double>> variety; 
		if (check.size() == 4) {
			variety.push_back({ check[0], check[1] });
			variety.push_back({ check[3], check[1] });
			variety.push_back({ check[2], check[0] });
			variety.push_back({ check[2], check[3] });
		}
		else if (check.size() == 3) {
			variety.push_back({ check[0], check[1] });
			variety.push_back({ check[2], check[0] });
		}
		/*else if (check.size() == 2) {
			variety.push_back({ check[0], check[1] });
			variety.push_back({ check[1], check[0] });
		}*/
		if (variety.size() == 4) {
			double buf_sum = (Sum_min_ij(variety[0]) + Sum_min_ij(variety[3]));
			double buf_sum1 = (Sum_min_ij(variety[1]) + Sum_min_ij(variety[2]));
			if (buf_sum >= buf_sum1) return { variety[0], variety[3] };
			else return { variety[1], variety[2] };
		}
		else {
			double buf_sum = Sum_min_ij(variety[0]);
			double buf_sum1 = Sum_min_ij(variety[1]);
			if (buf_sum >= buf_sum1) return { variety[0]};
			else return { variety[1]};
		}
	}

	double mid() {
		double middle_index = 0;
		for (int i = 0; i < Box.size(); i++) {
			middle_index += (i + 1);
		}
		middle_index = middle_index / Box.size();
		return middle_index - 1;
	}

	int pos(double middle, double index_number) { // 0 - <; 1 - >; 2 - ==;
		if (index_number < middle) return 0;
		else if (index_number > middle) return 1;
		else if (index_number == middle) return 2;
	}

	vector<double> Left_Alg(vector<double> buf_Box, vector<double> buf_Dinamic) {
		buf_Dinamic.push_back(*(buf_Box.end() - 1));
		buf_Box.erase((buf_Box.end() - 1));
		buf_Dinamic.insert(buf_Dinamic.begin(), *(buf_Box.end() - 1));
		buf_Box.erase((buf_Box.end() - 1));
		int iter = buf_Box.size() / 2;
		if (buf_Box.size() % 2 == 1) iter++;
		for (int i = 0; i < iter; i++) {
			vector<vector<double>> buf;
			if (buf_Box.size() != 1) {
				if (i % 2 == 0) {
					buf = Benefit({ buf_Box[0], *buf_Dinamic.begin(), *(buf_Dinamic.end() - 1) ,buf_Box[1] });
					buf_Dinamic.push_back(buf[1][1]);
					buf_Box.erase(buf_Box.begin());
					buf_Dinamic.insert(buf_Dinamic.begin(), buf[0][0]);
					buf_Box.erase(buf_Box.begin());
				}
				else {
					buf = Benefit({ *(buf_Box.end() - 1), *buf_Dinamic.begin(), *(buf_Dinamic.end() - 1) ,*(buf_Box.end() - 2) });
					buf_Dinamic.push_back(buf[1][1]);
					buf_Box.erase((buf_Box.end() - 1));
					buf_Dinamic.insert(buf_Dinamic.begin(), buf[0][0]);
					buf_Box.erase((buf_Box.end() - 1));
				}
			}
			else {
				buf = Benefit({ buf_Box[0], *buf_Dinamic.begin(), *(buf_Dinamic.end() - 1)});
				if (buf[0][1] == *buf_Dinamic.begin()) {
					buf_Dinamic.insert(buf_Dinamic.begin(), buf[0][0]);
					buf_Box.erase(buf_Box.begin());
				}
				else {
					buf_Dinamic.push_back(buf[0][1]);
					buf_Box.erase(buf_Box.begin());
				}
			}
		}
		return buf_Dinamic;
	}
	vector<double> Right_Alg(vector<double> buf_Box, vector<double> buf_Dinamic) {
		buf_Dinamic.insert(buf_Dinamic.begin(), *buf_Box.begin());
		buf_Box.erase(buf_Box.begin());
		buf_Dinamic.push_back(*buf_Box.begin());
		buf_Box.erase(buf_Box.begin());
		int iter = buf_Box.size() / 2;
		if (buf_Box.size() % 2 == 1) iter++;
		for (int i = 0; i < iter; i++) {
			vector<vector<double>> buf;
			if (buf_Box.size() != 1) {
				if (i % 2 == 1) {
					buf = Benefit({ *buf_Box.begin(), *buf_Dinamic.begin(), *(buf_Dinamic.end() - 1) , *(buf_Box.begin() + 1) });
					buf_Dinamic.push_back(buf[1][1]);
					buf_Box.erase(buf_Box.begin());
					buf_Dinamic.insert(buf_Dinamic.begin(), buf[0][0]);
					buf_Box.erase(buf_Box.begin());
				}
				else {
					buf = Benefit({ *(buf_Box.end() - 1), *buf_Dinamic.begin(), *(buf_Dinamic.end() - 1) ,*(buf_Box.end() - 2) });
					buf_Dinamic.push_back(buf[1][1]);
					buf_Box.erase((buf_Box.end() - 1));
					buf_Dinamic.insert(buf_Dinamic.begin(), buf[0][0]);
					buf_Box.erase((buf_Box.end() - 1));
				}
			}
			else {
				buf = Benefit({ buf_Box[0], *buf_Dinamic.begin(), *(buf_Dinamic.end() - 1)});
				if (buf[0][1] == *buf_Dinamic.begin()) {
					buf_Dinamic.insert(buf_Dinamic.begin(), buf[0][0]);
					buf_Box.erase(buf_Box.begin());
				}
				else {
					buf_Dinamic.push_back(buf[0][1]);
					buf_Box.erase(buf_Box.begin());
				}
			}
		}
		return buf_Dinamic;
	}


	vector<double> Dinamic_Create_vector(int n_index) {
		vector<double> buf_Box = Box;
		vector<double> buf_Dinamic = { Box[n_index] };
		vector<double> buf_Dinamic1 = { Box[n_index] };
		buf_Box.erase(buf_Box.begin() + n_index);
		if (Box.size() == 2) buf_Dinamic.push_back(buf_Box[0]);
		else {
			double middle = mid();
			int pos_begin = pos(middle, n_index);
			if (pos_begin == 0) {
				buf_Dinamic = Left_Alg(buf_Box, buf_Dinamic);
			}
			else if (pos_begin == 1) {
				buf_Dinamic = Right_Alg(buf_Box, buf_Dinamic);
			}
			else {
				buf_Dinamic = Left_Alg(buf_Box, buf_Dinamic);
				buf_Dinamic1 = Right_Alg(buf_Box, buf_Dinamic1);
				if (Sum_min_ij(buf_Dinamic1) < Sum_min_ij(buf_Dinamic)) buf_Dinamic = buf_Dinamic1;
			}
		}
		return buf_Dinamic;
	}

	void Sum_min_function_Dinamic() {
		double buf_sum;
		for (int i = 0; i < Box.size(); i++) {
			vector<double> i_box;
			i_box = Dinamic_Create_vector(i);
			buf_sum = Sum_min_ij(i_box);
			if (Sum_min > buf_sum) {
				Box_min = i_box;
				Sum_min = buf_sum;
			}
		}
	}
	vector<double> recursion_sort_Dinamic(vector<double> box) {
		double buf_sum_l, buf_sum_r;
		vector<double> Buf;
		if (box.size() <= 2) return box;
		else {
			int middle = box.size() / 2;
			vector<double> Box_left(box.begin(), box.begin() + middle);
			Box_left = recursion_sort_Dinamic(Box_left);
			Buf = Box_left;
			vector<double> Box_right(box.begin() + middle, box.end());
			Box_right = recursion_sort_Dinamic(Box_right);
			Box_left.insert(Box_left.end(), Box_right.begin(), Box_right.end());
			Box_right.insert(Box_right.end(), Buf.begin(), Buf.end());
			if (Sum_min_ij(Box_left) < Sum_min_ij(Box_right)) return Box_left;
			else return Box_right;
		}
	}
	vector<double> swap_min(vector<double> buf_box) {
		int iter = buf_box.size() / 2;
		if (buf_box.size() % 2 == 0) iter--;
		for (int i = 0; i < iter; i++) {
			buf_box.insert(buf_box.begin() + (2 * i + 1), *(buf_box.end() - 1));
			buf_box.erase(buf_box.end() - 1);
		}
		return buf_box;
	}
	void Sum_min_function_Dinamic_sort() {
		double buf_sum;
		for (int i = 0; i < Box.size(); i++) {
			vector<double> box_buf = recursion_sort_Dinamic(Box);
			while (*(Box_memory.end() - 1) != box_buf) {
				Box_memory.push_back(box_buf);
				box_buf = recursion_sort_Dinamic(Box_memory[Box_memory.size() - 1]);
			}
			buf_sum = Sum_min_ij(box_buf);
			if (Sum_min > buf_sum) {
				Box_min = box_buf;
				Sum_min = buf_sum;
			}
			Box_memory = { { } };
			Box = swap_min(Box);
		}
	}
	int Position_x_i(int index) {
		double buf_sum = 100000, buf;
		int pos = 0;
		for (int i = 1; i < Box.size() + 1; i++) {
			vector<double> buf_box = *(Box_memory.end() - 1);
			buf_box.insert(buf_box.begin() + i, buf_box[index]);
			buf_box.erase(buf_box.begin() + index);
			buf = Sum_min_ij(buf_box);
			if (buf <= buf_sum) {
				pos = i; 
				buf_sum = buf;
			}
		}
		return pos;
	}
	void true_Sum_min_function_Dinamic() {
		Box_memory.push_back(Box);
		vector<double> buf_box = Box;
		int index = 0, buf_index;
		for(int i = 0; i < Box.size(); i++){
			buf_index = Position_x_i(index);
			if (buf_index != index) {
				buf_box.insert(buf_box.begin() + buf_index, buf_box[index]);
				buf_box.erase(buf_box.begin() + index);
				Box_memory.push_back(buf_box);
				if (buf_index == 0) index = 1;
				else index = 0;
			}
			index++;
		}
		Box_min = buf_box;
		Sum_min = Sum_min_ij(Box_min);
	}

public:
	void Box_Task(int d) {
		if (d < 2 || d > 6) {
			cout << "Incorrect number of disks. Disks amount must be in interval from 2 to 6" << endl;
			return;
		}
		disks = d;
		double buf;
		cout << "Input disks for " << disks << " disks" << endl;
		for (int i = 0; i < disks; i++) {
			cin >> buf;
			if (buf <= 0 || buf > 1000) {
				cout << "Disks have incorrect radius." << endl;
				Box.clear();
				cin.clear();
				cin.ignore(1000, '\n');
				return;
			}
			Box.push_back(buf);
		}
		Sum_min_function();
		Output_result();
		cin.clear();
		cin.ignore(1000, '\n');
	}
	void Box_Task_Dinamic(int d) {
		if (d < 2 || d > 6) {
			cout << "Incorrect number of disks. Disks amount must be in interval from 2 to 6" << endl;
			return;
		}
		disks = d;
		double buf;
		cout << "Input disks for " << disks << " disks" << endl;
		for (int i = 0; i < disks; i++) {
			cin >> buf;
			if (buf <= 0 || buf > 1000) {
				cout << "Disks have incorrect radius." << endl;
				Box.clear();
				cin.clear();
				cin.ignore(1000, '\n');
				return;
			}
			Box.push_back(buf);
		}
		sort(Box.begin(), Box.end());
		Sum_min_function_Dinamic();
		Output_result();
		cin.clear();
		cin.ignore(1000, '\n');
	}
	void Box_Task_Dinamic_sort(int d) {
		if (d < 2 || d > 6) {
			cout << "Incorrect number of disks. Disks amount must be in interval from 2 to 6" << endl;
			return;
		}
		disks = d;
		double buf;
		cout << "Input disks for " << disks << " disks" << endl;
		for (int i = 0; i < disks; i++) {
			cin >> buf;
			if (buf <= 0 || buf > 1000) {
				cout << "Disks have incorrect radius." << endl;
				Box.clear();
				cin.clear();
				cin.ignore(1000, '\n');
				return;
			}
			Box.push_back(buf);
		}
		sort(Box.begin(), Box.end());
		Sum_min_function_Dinamic_sort();
		Output_result();
		cin.clear();
		cin.ignore(1000, '\n');
	}
	void true_Box_Task_Dinamic(int d) {
		if (d < 2 || d > 6) {
			cout << "Incorrect number of disks. Disks amount must be in interval from 2 to 6" << endl;
			return;
		}
		disks = d;
		double buf;
		cout << "Input disks for " << disks << " disks" << endl;
		for (int i = 0; i < disks; i++) {
			cin >> buf;
			if (buf <= 0 || buf > 1000) {
				cout << "Disks have incorrect radius." << endl;
				Box.clear();
				cin.clear();
				cin.ignore(1000, '\n');
				return;
			}
			Box.push_back(buf);
		}
		sort(Box.begin(), Box.end());
		reverse(Box.begin(), Box.end());
		true_Sum_min_function_Dinamic();
		sort(Box.begin(), Box.end());
		Sum_min_function_Dinamic();
		Output_result();
		cin.clear();
		cin.ignore(1000, '\n');
	}
};
int main() {
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	int exit = 1;
	while (exit == 1) {
		int disks=0, task=0;
		BOX Box;
		cout << "1.Box_Task" << endl << "2.Box_Task_Dinamic"  << endl << "3.Box_Task_Dinamic_sort" << endl << "4.true_Box_Task_Dinamic" << endl << "5.Exit" << endl << "Choose a way:" << endl;
		cin >> task;
		switch (task)
		{
		case 1:
			cout << "Input number of disks:" << endl << "disks = ";
			cin >> disks;
			Box.Box_Task(disks);
			break;
		case 2:
			cout << "Input number of disks:" << endl << "disks = ";
			cin >> disks;
			Box.Box_Task_Dinamic(disks);
			break;
		case 3:
			cout << "Input number of disks:" << endl << "disks = ";
			cin >> disks;
			Box.Box_Task_Dinamic_sort(disks);
			break;
		case 4:
			cout << "Input number of disks:" << endl << "disks = ";
			cin >> disks;
			Box.true_Box_Task_Dinamic(disks);
			break;
		case 5:
			exit = 0;
			break;
		default:
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "This task does not exist" << endl;
			break;
		}
	}
	system("pause");
	return 0;
}

/*
 3
 1 2 2
*/