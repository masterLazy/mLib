#pragma once
/******************************************************
* lin_algebra
* ���Դ���
*
******************************************************/


/******************************************************
* Matrix
* ����
*
******************************************************/
class Matrix
{
private:
	std::vector<Math_F> arr;
	size_t row = 0, col = 0;
	Math_F over_range = 0;//Խ��ר�÷���
public:
	Matrix() _RST {}
	Matrix(const size_t _row, const size_t _col) _RST
	{
		row = _row;
		col = _col;
		arr.resize(row * col);
	}

	//��ȡ�С���
	size_t get_row() _RST
	{
		return row;
	}
	size_t get_col() _RST
	{
		return col;
	}

	//���
	void clear() _RST
	{
		arr.clear();
		row = col = 0;
	}

	//�Ĵ�С������գ�
	void resize(const size_t _row, const size_t _col) _RST
	{
		clear();
		row = _row;
		col = _col;
		arr.resize(row * col);
	}

	//����
	Math_F get(const size_t _row, const size_t _col) const _RST
	{
		//Խ����
		if (_row >= row || _row < 0 || _col >= col || _col < 0)
		{
			return 0;
		}
		return arr[_row * col + _col];
	}
	Math_F& operator()(const size_t _row, const size_t _col) _RST
	{
		//Խ����
		if (_row >= row || _row < 0 || _col >= col || _col < 0)
		{
			over_range = 0;
			return over_range;
		}
		return arr[_row * col + _col];
	}

	//ȫ�����Ϊ
	void fill(const Math_F a) _RST
	{
		for (size_t i = 0; i < arr.size(); i++)
		{
			arr[i] = a;
		}
	}


	//�ӷ����޶����򷵻ؿվ���
	Matrix operator+(const Matrix& m) const _RST
	{
		Matrix res;
		if (m.row == row && m.col == col)
		{
			for (size_t i = 0; i < res.arr.size(); i++)
			{
				res.arr[i] += m.arr[i];
			}
		}
		return res;
	}
	Matrix operator+=(const Matrix& m) _RST
	{
		if (m.row == row && m.col == col)
		{
			for (size_t i = 0; i < arr.size(); i++)
			{
				arr[i] += m.arr[i];
			}
		}
		return *this;
	}
	Matrix friend operator+(const Matrix& a, const Math_F b)
	{
		Matrix res = a;
		for (size_t i = 0; i < res.arr.size(); i++)
		{
			res.arr[i] += b;
		}
		return res;
	}

	//�������޶����򷵻ؿվ���
	Matrix operator-(const Matrix& m) const _RST
	{
		Matrix res;
		if (m.row == row && m.col == col)
		{
			for (size_t i = 0; i < res.arr.size(); i++)
			{
				res.arr[i] -= m.arr[i];
			}
		}
		return res;
	}
	Matrix operator-=(const Matrix& m) _RST
	{
		if (m.row == row && m.col == col)
		{
			for (size_t i = 0; i < arr.size(); i++)
			{
				arr[i] -= m.arr[i];
			}
		}
		return *this;
	}
	Matrix friend operator-(const Matrix& a, const Math_F b)
	{
		Matrix res = a;
		for (size_t i = 0; i < res.arr.size(); i++)
		{
			res.arr[i] -= b;
		}
		return res;
	}

	//����
	Matrix operator*(const Math_F a) const _RST
	{
		Matrix res = *this;
		for (size_t i = 0; i < res.arr.size(); i++)
		{
			res.arr[i] *= a;
		}
		return res;
	}
	Matrix operator*=(const Math_F a) _RST
	{
		for (size_t i = 0; i < arr.size(); i++)
		{
			arr[i] *= a;
		}
		return *this;
	}
	Matrix friend operator*(const Matrix& m, const Math_F a)
	{
		Matrix res = m;
		for (size_t i = 0; i < res.arr.size(); i++)
		{
			res.arr[i] *= a;
		}
		return res;
	}

	//����˷����޶����򷵻ؿվ���
	Matrix operator*(const Matrix& m)
	{
		Matrix res(row, m.col);
		if (m.row == col && m.col == row)
		{
			for (size_t i = 0; i < res.row; i++)
			{
				for (size_t j = 0; j < res.col; j++)
				{
					for (size_t k = 0; k < col; k++)
					{
						res(i, j) += get(i, k) * m.get(k, j);
					}
				}
			}
		}
		return res;
	}
};