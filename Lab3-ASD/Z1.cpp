#include <iostream>
#include "windows.h"
#include <vector>
#include <iterator>

using namespace std;

class Stocks {
private:
	int days;
	vector<int> Stocks;
	vector<int> Diff;

	vector<int> Merge_Cross(int low, int hight) {
		int mid = (low + hight) / 2;
		int left_sum = INT_MIN, sum = 0, max_left = mid;
		for (int i = mid; i >= low; i--) {
			sum += Diff[i];
			if (sum > left_sum) {
				left_sum = sum;
				max_left = i;
			}
		}
		int right_sum = INT_MIN, max_right = mid;
		sum = 0;
		for (int i = mid + 1; i <= hight; i++) {
			sum += Diff[i];
			if (sum > right_sum) {
				right_sum = sum;
				max_right = i;
			}
		}
		vector<int> result = { max_left, max_right, (left_sum + right_sum) };
		return result;
	}
	vector<int> Merge(int low, int hight) {
		if (low == hight) {
			vector<int> result = { low, hight, Diff[low] }; //low, hight, mid
			return result;
		}
		else {
			vector<int> result, left_result, right_result, cross_result;
			int mid = 0;
			if (low == hight - 1) mid = (low + hight - 1) / 2;
			else mid = (low + hight) / 2;
			left_result = Merge(low, mid);
			right_result = Merge(mid+1, hight);
			cross_result = Merge_Cross(low, hight);
			if (left_result[2] >= right_result[2] && left_result[2] >= cross_result[2]) {
				result = left_result;
				return result;
			}
			else if (right_result[2] >= left_result[2] && right_result[2] >= cross_result[2]) {
				result = right_result;
				return result;
			}
			else {
				result = cross_result;
				return result;
			}
		}
	}
public:
	void Stock_Growth(int d) {
		if (d < 2) {
			cout << "Incorrect number of days. Days must be 2 or more" << endl;
			return;
		}
		days = d;
		int buf;
		cout << "Input stocks for " << days << " days" << endl;
		for (int i = 0; i < days; i++) {
			cin >> buf;
			if (buf < 0) {
				cout << "Stocks cannot be less than 0. Try again:" << endl;
				i = 0;
				Stocks.clear();
			}
			Stocks.push_back(buf);
		}
		for (vector<int> ::iterator it = Stocks.begin(); it < Stocks.end() - 1; it++) {
			Diff.push_back(*(it + 1) - *it);
		}
		vector<int> result = Merge(0, Diff.size() - 1);
		cout << "Day of purchase: " << (result[0] + 1) << ". Day of sale: " << (result[1] + 2) << ". Margin: " << result[2] << "." << endl;
		//индекс+1, начало 0, конец +1;
	}
};
int main() {
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	int days, task, exit = 1;
	while (exit == 1) {
		Stocks St;
		cout << "1.Stock_Growth" << endl << "2.Exit" << endl << "Choose a way:" << endl;
		cin >> task;
		switch (task)
		{
		case 1:
			cout << "Input number of days:" << endl << "days = ";
			cin >> days;
			St.Stock_Growth(days);
			break;
		case 2:
			exit = 0;
			break;
		default:
			cout << "This task does not exist" << endl;
			break;
		}
	}
	system("pause");
	return 0;
}
// 1) 17 days: 100 113 110 85 105 102 86 63 81 101 94 106 101 79 94 90 97