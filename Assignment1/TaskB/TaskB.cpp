#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

using namespace std;

// Define a class for complex numbers
class Complex {
private:
    double real; // Real part of the complex number
    double imaginary; // Imaginary part of the complex number

public:
    // Constructor to initialize the real and imaginary parts
    Complex(double r, double i) : real(r), imaginary(i) {}

    // Overload the addition operator (+) for complex numbers
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imaginary + other.imaginary);
    }

    // Overload the subtraction operator (-) for complex numbers
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imaginary - other.imaginary);
    }

    // Overload the multiplication operator (*) for complex numbers
    Complex operator*(const Complex& other) const {
        return Complex(real * other.real - imaginary * other.imaginary,
                       real * other.imaginary + imaginary * other.real);
    }

    // Overload the division operator (/) for complex numbers
    Complex operator/(const Complex& other) const {
        double denominator = other.real * other.real + other.imaginary * other.imaginary;
        return Complex((real * other.real + imaginary * other.imaginary) / denominator,
                       (imaginary * other.real - real * other.imaginary) / denominator);
    }

    // Overload the equality operator (==) for complex numbers
    bool operator==(const Complex& other) const {
        return (real == other.real) && (imaginary == other.imaginary);
    }

    // Overload the inequality operator (!=) for complex numbers
    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

    // Overload the << operator to output a complex number
    friend ostream& operator<<(ostream& out, const Complex& c) {
        out << fixed << setprecision(2) << c.real; // Output real part with 2 decimal places
        if (c.imaginary >= 0)
            out << "+"; // Output "+" if imaginary part is positive
        out << fixed << setprecision(2) << c.imaginary << "i"; // Output imaginary part with 2 decimal places and "i"
        return out;
    }
};

int main() {
    int n;
    cin >> n;

    // Iterate through each operation
    for (int i = 0; i < n; ++i) {
        char op;
        double a, b, c, d;
        cin >> op >> a >> b >> c >> d; // Read the operation and complex numbers

        Complex num1(a, b); // Create first complex number
        Complex num2(c, d); // Create second complex number
        Complex result(0, 0); // Initialize result complex number

        switch (op) {
            case '+': // Addition operation
                result = num1 + num2;
                break;
            case '-': // Subtraction operation
                result = num1 - num2;
                break;
            case '*': // Multiplication operation
                result = num1 * num2;
                break;
            case '/': // Division operation
                result = num1 / num2;
                break;
            case '=': // Equality check
                cout << (num1 == num2 ? "true" : "false") << endl; // Output true if equal, otherwise false
                continue; // Skip the rest of the loop
        }

        cout << result << endl; // Output the result of the operation
    }

    return 0;
}
