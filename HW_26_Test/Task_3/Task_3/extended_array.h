#pragma once
#include <utility>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T>
class ExtArray
{
private:
	std::vector<T> extended_array;
	size_t _size;
public:
	ExtArray(std::initializer_list<T> l) : extended_array(l)
	{
		_size = l.size();
	}

	ExtArray(int size) : _size(size)
	{
		extended_array.resize(_size);
	}

	T& operator[](size_t index)
	{
		return extended_array[index];
	}

	size_t size()
	{
		return _size;
	}

	double mean()
	{
		double sum = 0;
		for (size_t i = 0; i < _size; i++)
		{
			sum += extended_array[i];
		}
		return sum / _size;
	}

	double median()
	{
		std::vector<T> temp_array;
		std::copy(extended_array.begin(), extended_array.end(), back_inserter(temp_array));
		std::sort(temp_array.begin(), temp_array.end());
		if (_size % 2 == 1)
		{
			return temp_array[_size / 2];
		}
		else
		{
			return static_cast<double>(temp_array[(_size / 2) - 1] + temp_array[_size / 2]) / 2;
		}
	}

	std::pair<T, int> mode()
	{
		T max = extended_array[0], cmax = 0, rmax = 0;
		for (int i = 0; i < _size; i++)
		{
			if (cmax > rmax)
			{
				rmax = cmax;
				max = extended_array[i - 1];
			}
			cmax = 0;
			for (int j = i; j < _size; j++)
			{
				if (extended_array[j] == extended_array[i])
				{
					cmax++;
				}
			}
		}
		return std::pair<T, int>(max, rmax);
	}

	int checkSum()
	{
		if (_size == 0) return 0;
		//ExtArray<int> extended_array{};
		int true_cnt = 0;
		for (size_t i = 0; i < _size; i++)
		{
			try {
				//if (typeid(extended_array[i]).name() != "int" && typeid(extended_array[i]).name() != "bool") {
				if (!checkTypes(extended_array[i])) {
					throw std::bad_typeid();
				}
			}
			catch (std::bad_typeid& terr) {
				return -1;
			}
			
			if (checkTypes(extended_array[i])) {
				try {
					if (extended_array[i] != 0 && extended_array[i] != 1) {
						throw std::logic_error("Массив содержит элементы, отличные от 0/False и 1/true");
					}
				}
			
				catch (std::logic_error& lerr) {
					return -2;
				}
			
				if (extended_array[i] == 1) {
					true_cnt++;
				}
			}
		}
		return true_cnt;
	}

	bool checkTypes(const T& element) {
		return std::is_same_v<T, int> || std::is_same_v<T, bool>;
	}



};
