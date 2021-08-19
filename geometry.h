//
// Created by Administrator on 2021/6/2.
//

#ifndef CGSTUDY_GEOMETRY_H
#define CGSTUDY_GEOMETRY_H

#include <iostream>
#include <cassert>
#include <initializer_list>

#include <cmath>

using namespace std;

//������
template<int n, typename T>
class Vec {
public:
    T data_[n] = {0};

    Vec() = default;

    Vec(const Vec<n, T> &src) {
        for (int i = 0; i < n; i++) {
            data_[i] = src.data_[i];
        }
    }

    Vec(const std::initializer_list<T> &src) {
        auto it = src.begin();
        for (int i = 0; i < n; i++) {
            data_[i] = *it;
            it++;
        }
    }


    Vec(const T *ptr) {
        for (int i = 0; i < n; i++) {
            data_[i] = ptr[i];
        }
    }

    T &operator[](const int i) {
        //assert(0 <= i && i < n);
        return data_[i];
    }

    const T &operator[](const int i) const {
        //assert(0 <= i && i < n);
        return data_[i];
    }

    T &x() {
        //assert(n >= 1);
        return data_[0];
    }

    const T &x() const {
        //assert(n >= 1);
        return data_[0];
    }

    T &y() {
        //assert(n >= 2);
        return data_[1];
    }

    const T &y() const {
        //assert(n >= 2);
        return data_[1];
    }

    T &z() {
        //assert(n >= 3);
        return data_[2];
    }

    const T &z() const {
        //assert(n >= 3);
        return data_[2];
    }

    T &w() {
        //assert(n >= 4);
        return data_[3];
    }

    const T &w() const {
        //assert(n >= 4);
        return data_[3];
    }

};

//-------------------------------------------------------------------------------------------------
//���������������

// ����


// ���
template<int n, typename T>
ostream &operator<<(ostream &os, const Vec<n, T> &a) {
    os << '[';
    for (int i = 0; i < n - 1; i++) {
        os << a[i] << ", ";
    }
    os << a[n - 1] << ']';
    return os;
}




//-------------------------------------------------------------------------------------------------
//����������ͨ����

// +a
template<int n, typename T>
Vec<n, T> operator+(const Vec<n, T> &a) {
    return a;
}

// -a
template<int n, typename T>
Vec<n, T> operator-(const Vec<n, T> &a) {
    Vec<n, T> ret;
    for (int i = 0; i < n; i++) {
        ret[i] = -a[i];
    }
    return ret;
}

// a == b
template<int n, typename T>
bool operator==(const Vec<n, T> &a, const Vec<n, T> &b) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

// a != b
template<int n, typename T>
bool operator!=(const Vec<n, T> &a, const Vec<n, T> &b) {
    return !(a == b);
}

// a + b
template<int n, typename T>
Vec<n, T> operator+(const Vec<n, T> &a, const Vec<n, T> &b) {
    Vec<n, T> ret;
    for (int i = 0; i < n; i++) {
        ret[i] = a[i] + b[i];
    }
    return ret;
}

// a - b
template<int n, typename T>
Vec<n, T> operator-(const Vec<n, T> &a, const Vec<n, T> &b) {
    Vec<n, T> ret;
    for (int i = 0; i < n; i++) {
        ret[i] = a[i] - b[i];
    }
    return ret;
}

// a * b���ڻ�
template<int n, typename T>
T operator*(const Vec<n, T> &a, const Vec<n, T> &b) {
    return dotProduct(a, b);
}

// a / b������Ԫ�����
template<int n, typename T>
Vec<n, T> operator/(const Vec<n, T> &a, const Vec<n, T> &b) {
    Vec<n, T> ret;
    for (int i = 0; i < n; i++) {
        ret[i] = a[i] / b[i];
    }
    return ret;
}


// x * a, ��������
template<int n, typename T>
Vec<n, T> operator*(T x, const Vec<n, T> &a) {
    Vec<n, T> ret;
    for (int i = 0; i < n; i++) {
        ret[i] = a[i] * x;
    }
    return ret;
}

//  a * x����������
template<int n, typename T>
Vec<n, T> operator*(const Vec<n, T> &a, T x) {
    return x * a;
}


// a / x
template<int n, typename T>
Vec<n, T> operator/(const Vec<n, T> &a, T x) {
    Vec<n, T> ret;
    for (int i = 0; i < n; i++) {
        ret[i] = a[i] / x;
    }
    return ret;
}

// x / a
template<int n, typename T>
Vec<n, T> operator/(T x, const Vec<n, T> &a) {
    Vec<n, T> ret;
    for (int i = 0; i < n; i++) {
        ret[i] = x / a[i];
    }
    return ret;
}

// a += b
template<int n, typename T>
Vec<n, T> &operator+=(Vec<n, T> &a, const Vec<n, T> &b) {
    for (int i = 0; i < n; i++) {
        a[i] += b[i];
    }
    return a;
}

// a -= b
template<int n, typename T>
Vec<n, T> &operator-=(Vec<n, T> &a, const Vec<n, T> &b) {
    for (int i = 0; i < n; i++) {
        a[i] -= b[i];
    }
    return a;
}


// a /= b
template<int n, typename T>
Vec<n, T> &operator/=(Vec<n, T> &a, const Vec<n, T> &b) {
    for (int i = 0; i < n; i++) {
        a[i] /= b[i];
    }
    return a;
}

// a *= x
template<int n, typename T>
Vec<n, T> &operator*=(Vec<n, T> &a, T x) {
    for (int i = 0; i < n; i++) {
        a[i] *= x;
    }
    return a;
}

// a /= x
template<int n, typename T>
Vec<n, T> &operator/=(Vec<n, T> &a, T x) {
    for (int i = 0; i < n; i++) {
        a[i] /= x;
    }
    return a;
}

//-------------------------------------------------------------------------------------------------
//������������


// ������Ԫ�س˷�
template<int n, typename T>
Vec<n, T> componentWiseProduct(const Vec<n, T> &a, const Vec<n, T> &b) {
    Vec<n, T> ret;
    for (int i = 0; i < n; i++) {
        ret[i] = a[i] * b[i];
    }
    return ret;
}


// �������ڻ�, ����
template<int n, typename T>
T dotProduct(const Vec<n, T> &a, const Vec<n, T> &b) {
    T ret = 0;
    for (int i = 0; i < n; i++) {
        ret += a[i] * b[i];
    }
    return ret;
}


// �����Ĳ����������������ά
// ��ά������ˣ��õ�����
template<typename T>
T crossProduct(const Vec<2, T> &a, const Vec<2, T> &b) {
    return a.x() * b.y() - a.y() * b.x();
}

// ��ά������ˣ��õ�������
template<typename T>
Vec<3, T> crossProduct(const Vec<3, T> &a, const Vec<3, T> &b) {
    return Vec<3, T>{a.y() * b.z() - a.z() * b.y(),
                     a.z() * b.x() - a.x() * b.z(),
                     a.x() * b.y() - a.y() * b.x()};
}

// ��ά������ˣ�ǰ��ά��ˣ���һά����
template<typename T>
Vec<4, T> crossProduct(const Vec<4, T> &a, const Vec<4, T> &b) {
    return Vec<4, T>{a.y() * b.z() - a.z() * b.y(),
                     a.z() * b.x() - a.x() * b.z(),
                     a.x() * b.y() - a.y() * b.x(),
                     a.w()};
}

// |a|^2, ������ģ����ƽ��
template<int n, typename T>
T vecModulusSquare(const Vec<n, T> &a) {
    return dotProduct(a, a);
}

// |a|, ������ģ��
template<int n, typename T>
T vecModulus(const Vec<n, T> &a) {
    return sqrt(dotProduct(a, a));
}

// |a| ,�ػ� float����sqrtf
template<int n>
float vecModulus(const Vec<n, float>& a) {
    return sqrtf(dotProduct(a, a));
}

// ���� a ��λ���Ľ��
template<int n, typename T>
Vec<n, T> vecNormalized(const Vec<n, T> &a) {
    return a / vecModulus(a);
}

// ���� a ��λ��Ľ��
template<int n, typename T>
Vec<n, T> vecHomogenized(const Vec<n, T> &a) {
    T last_d = a.data_[n - 1];
    return a / last_d;
}



// ���� a ��ά�Ľ��
template<int n, typename T>
Vec<n + 1, T> vecDimensionIncreased(const Vec<n, T> &a) {
    Vec<n + 1, T> ret;
    for (int i = 0; i < n; i++) {
        ret.data_[i] = a.data_[i];
    }
    ret.data_[n] = 1;
    return ret;
}

// ���� a ֱ�ӽ�ά�Ľ�������ù����һά
template<int n, typename T>
Vec<n - 1, T> vecDimensionReducedDir(const Vec<n, T> &a) {
    Vec<n - 1, T> ret;
    for (int i = 0; i < n - 1; i++) {
        ret.data_[i] = a.data_[i];
    }
    return ret;
}


// ���� a �ȱ��������꣨���һά���1���ٽ�ά�Ľ��
template<int n, typename T>
Vec<n - 1, T> vecDimensionReducedHom(const Vec<n, T> &a) {
    Vec<n - 1, T> ret;
    T last_d = a.data_[n - 1];
    for (int i = 0; i < n - 1; i++) {
        ret.data_[i] = a.data_[i] / last_d;
    }
    return ret;
}

//-------------------------------------------------------------------------------------------------
// ����ת��
template<int n, typename T>
Vec<n, float> vecToFloat(const Vec<n, T> &a) {
    Vec<n, float> ret;
    for (int i = 0; i < n; i++) {
        ret.data_[i] = static_cast<float>(a.data_[i]);
    }
    return ret;
}

template<int n, typename T>
Vec<n, double> vecToDouble(const Vec<n, T> &a) {
    Vec<n, double> ret;
    for (int i = 0; i < n; i++) {
        ret.data_[i] = static_cast<double>(a.data_[i]);
    }
    return ret;
}

template<int n, typename T>
Vec<n, int> vecToInt(const Vec<n, T> &a) {
    Vec<n, int> ret;
    for (int i = 0; i < n; i++) {
        ret.data_[i] = lround(a.data_[i]);
    }
    return ret;
}



//-------------------------------------------------------------------------------------------------
//������
template<int m, int n, typename T>
class Mat {
public:
    T data_[m][n] = {0};

    Mat() = default;

    Mat(const Mat<m, n, T> &src) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                data_[i][j] = src.data_[i][j];
            }
        }
    }

    Mat(const initializer_list<Vec<n, T>> &src) {
        auto it = src.begin();
        for (int i = 0; i < m; i++) {
            setRow(i, *it);
            it++;
        }
    }

    T *operator[](const int i) {
        //assert(0 <= i && i < m);
        return data_[i];
    }

    const T *operator[](const int i) const {
        //assert(0 <= i && i < m);
        return data_[i];
    }


    // ȡһ��
    Vec<n, T> getRow(int row) const {
        //assert(row < m);
        Vec<n, T> ret;
        for (int j = 0; j < n; j++) {
            ret[j] = data_[row][j];
        }
        return ret;
    }

    // ȡһ��
    Vec<m, T> getCol(int col) const {
        //assert(col < n);
        Vec<m, T> ret;
        for (int i = 0; i < m; i++) {
            ret[i] = data_[i][col];
        }
        return ret;
    }

    // ����һ��
    void setRow(int row, const Vec<n, T> &a) {
        //assert(row < m);
        for (int i = 0; i < n; i++) {
            data_[row][i] = a[i];
        }
    }

    // ����һ��
    void setCol(int col, const Vec<m, T> &a) {
        //assert(col < n);
        for (int i = 0; i < m; i++) {
            data_[i][col] = a[i];
        }
    }

};


//-------------------------------------------------------------------------------------------------
//���ؾ����������

// ���
template<int m, int n, typename T>
ostream &operator<<(ostream &os, const Mat<m, n, T> &a) {
    for (int i = 0; i < m; i++) {
        if (i == 0) {
            os << '[';
        } else {
            os << ' ';
        }

        Vec<n, T> ai = a[i];

        os << ai;

        if (i == m - 1) {
            os << ']';
        } else {
            os << endl;
        }
    }
    return os;
}


//-------------------------------------------------------------------------------------------------
// ������ͨ����

// a == b
template<int m, int n, typename T>
bool operator==(const Mat<m, n, T> &a, const Mat<m, n, T> &b) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (a.m[i][j] != b.m[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// a != b
template<int m, int n, typename T>
bool operator!=(const Mat<m, n, T> &a, const Mat<m, n, T> &b) {
    return !(a == b);
}

// +a
template<int m, int n, typename T>
Mat<m, n, T> operator+(const Mat<m, n, T> &src) {
    return src;
}

// -a
template<int m, int n, typename T>
Mat<m, n, T> operator-(const Mat<m, n, T> &src) {
    Mat<m, n, T> ret;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ret.data_[i][j] = -src.data_[i][j];
        }
    }
    return ret;
}

// a + b
template<int m, int n, typename T>
Mat<m, n, T> operator+(const Mat<m, n, T> &a, const Mat<m, n, T> &b) {
    Mat<m, n, T> ret;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ret.data_[i][j] = a.data_[i][j] + b.data_[i][j];
        }
    }
    return ret;
}

// a - b
template<int m, int n, typename T>
Mat<m, n, T> operator-(const Mat<m, n, T> &a, const Mat<m, n, T> &b) {
    Mat<m, n, T> ret;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ret.data_[i][j] = a.data_[i][j] - b.data_[i][j];
        }
    }
    return ret;
}


// x * a������������
template<int m, int n, typename T>
Mat<m, n, T> operator*(T x, const Mat<m, n, T> &a) {
    Mat<m, n, T> ret;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ret.data_[i][j] = x * a.data_[i][j];
        }
    }
    return ret;
}

// a * x
template<int m, int n, typename T>
Mat<m, n, T> operator*(const Mat<m, n, T> &a, T x) {
    return x * a;
}

// a / x
template<int m, int n, typename T>
Mat<m, n, T> operator/(const Mat<m, n, T> &a, T x) {
    Mat<m, n, T> ret;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ret.data_[i][j] = a.data_[i][j] / x;
        }
    }
    return ret;
}


// x / a�����һ���ò���
template<int m, int n, typename T>
Mat<m, n, T> operator/(T x, const Mat<m, n, T> &a) {
    Mat<m, n, T> ret;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ret.data_[i][j] = x / a.data_[i][j];
        }
    }
    return ret;
}


//-------------------------------------------------------------------------------------------------
// ��������������


//��������������(��ʵ��������������������)��a(1*m) * A(m*n) = ret(1*n)
template<int m, int n, typename T>
Vec<n, T> operator*(const Vec<m, T> &a, const Mat<m, n, T> &A) {
    Vec<n, T> ret;
    for (int j = 0; j < n; j++)
        ret[j] = dotProduct(a, A.getCol(j));
    return ret;
}

//��������������ˣ�A(m*n) * a(n*1) = ret(m*1)
template<int m, int n, typename T>
Vec<m, T> operator*(const Mat<m, n, T> &A, const Vec<n, T> &a) {
    Vec<m, T> ret;
    for (int i = 0; i < m; i++) {
        ret[i] = dotProduct(A.getRow(i), a);
    }
    return ret;
}

//-------------------------------------------------------------------------------------------------
// �������������


// a(m*n) * b(n*p) = ret(m*p)������ľ���˷����������ʱ���ͨд����
template<int m, int n, int p, typename T>
Mat<m, p, T> operator*(const Mat<m, n, T> &a, const Mat<n, p, T> &b) {
    Mat<m, p, T> ret;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < m; i++) {
            T r = a.data_[i][k];
            for (int j = 0; j < p; j++) {
                ret.data_[i][j] += r * b.data_[k][j];
            }
        }
    }
/*��ͨд��
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            T sum = 0;
            for (int k = 0; k < n; k++) {
                sum += a.data_[i][k] * b.data_[k][j];
            }
            ret.data_[i][j] = sum;
        }
    }
*/
    return ret;
}

//-------------------------------------------------------------------------------------------------
// ���ɾ���

// ��λ��
template<int n, typename T>
Mat<n, n, T> identityMat() {
    Mat<n, n, T> ret;
    for (int i = 0; i < n; i++) {
        ret.data_[i][i] = 1;
    }
    return ret;
}

// ת�þ���
template<int m, int n, typename T>
Mat<n, m, T> matTransposed(const Mat<m, n, T> &a) {
    Mat<n, m, T> ret;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ret.data_[j][i] = a.data_[i][j];
        }
    }
    return ret;
}

// �����
//template<int n, typename T>
//Mat<



//-------------------------------------------------------------------------------------------------
// ���ͱ���

using Vec2i = Vec<2, int>;
using Vec2f = Vec<2, float>;
using Vec2d = Vec<2, double>;

using Vec3i = Vec<3, int>;
using Vec3f = Vec<3, float>;
using Vec3d = Vec<3, double>;

using Vec4i = Vec<4, int>;
using Vec4f = Vec<4, float>;
using Vec4d = Vec<4, double>;

using Mat33f = Mat<3, 3, float>;
using Mat44f = Mat<4, 4, float>;
using Mat34f = Mat<3, 4, float>;
using Mat43f = Mat<4, 3, float>;

using Mat33d = Mat<3, 3, double>;
using Mat44d = Mat<4, 4, double>;
using Mat34d = Mat<3, 4, double>;
using Mat43d = Mat<4, 3, double>;


#endif //CGSTUDY_GEOMETRY_H
