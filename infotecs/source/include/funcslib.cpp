#include "funcs.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cctype>
#include <bits/stdc++.h>


using namespace std;



bool Functions::f3(string str) {
	int size = str.length();
	if (size > 2 && size % 32 == 0)
		return true;
	else return false;
}

int Functions::f2(string str) {
	int sum = 0;
	for (char i : str)
		if (isdigit(i) != 0)
		{
			int j = i - '0';
			sum += j;
		}
		
	return sum;
}

void Functions::f1(string str) {
	extern string buffer;
	vector<char> data(str.begin(), str.end());
	stable_sort(data.begin(), data.end(), greater<char>());
	for (auto i = str.length() - 1; i >= 1; i--)
	{
		if ((i + 1) % 2 == 0)
		{
			data[i] = 'K';
			data.insert(data.begin() + i + 1, 'B');

		}
	}
	string s(data.begin(), data.end());
	buffer = s;
	
}

