#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <queue>

template <typename T>
class Matrix
{
    public:
        class OutOfRangeException {};
        Matrix(int height = 0, int width = 0);
        ~Matrix();
        int getWidth() { return m_width; }
        int getHeight() { return m_height; }
        T GetAt(unsigned int rowNum, unsigned int colNum);
        T*& operator[](unsigned int i);
        T** GetMatrix() { return m_matrix; }
        void CopyFrom(T** matrix);

    private:
        T* m_data;
        T** m_matrix;
        int m_height;
        int m_width;
};

template <typename T>
Matrix<T>::Matrix(int height, int width)
{
    if (width == 0 || height == 0)
    {
        m_width = m_height = 0;
    }
    else
    {
        m_width = width;
        m_height = height;
        m_data = new T [width*height];
        m_matrix = new T* [height];
        m_matrix[0] = &m_data[0];
        for (int i = 1; i < height; i++)
        {
            m_matrix[i] = m_matrix[i - 1] + width;
        }
    }
}

template <typename T>
Matrix<T>::~Matrix()
{
    if (m_width != 0 && m_height != 0)
    {
        if (m_data != NULL)
            delete [] m_data;
        delete [] m_matrix;
    }
}

template <typename T>
T Matrix<T>::GetAt(unsigned int rowNum, unsigned int colNum)
{
    if (rowNum >= m_height || colNum >= m_width)
        throw OutOfRangeException;
    return m_matrix[rowNum][colNum];
}

template <typename T>
T*& Matrix<T>::operator[](unsigned int i)
{
    if (i >= m_height)
        throw OutOfRangeException;
	return m_matrix[i];
}

template <typename T>
void Matrix<T>::CopyFrom(T** matrix)
{
    for (int i = 0; i < m_height; i++)
        for (int j = 0; j < m_width; j++)
            m_matrix[i][j] = matrix[i][j];
}

#endif // MATRIX_H_INCLUDED
