// doubles and matrix to do the math
// iostream to print the results

#include <cpp11/doubles.hpp>
#include <cpp11/matrix.hpp>
#include <iostream>

using namespace cpp11;
using namespace std;

// this function has no output
// i.e., it won't send a matrix or vector back to R
// instead, it will print the results to the console

[[cpp11::register]] void cpp11_simplex_phase2_(doubles c, doubles b, doubles_matrix<> A) {
    // Naive implementation of the simplex algorithm (phase 2)

    // Check dimensions
    int m = A.nrow();
    int n = A.ncol();

    if (m != b.size())
    {
        stop("incompatible dimensions between A and b");
    }

    if (n != c.size())
    {
        stop("incompatible dimensions between A and c");
    }

    // Create the tableau
    // DON'T FORGET TO USE writable:: TO MAKE IT MODIFIABLE
    writable::doubles_matrix<> T(m + 1, n + m + 1);

    // b2 = [0, b]
    writable::doubles b2(m + 1);

    // fill b2_i+1 with b_i
    for (int i = 0; i < m; i++)
    {   
        b2[i] = 0.0;
    }
    for (int i = 0; i < m; i++)
    {   
        b2[i + 1] += b[i];
    }

    // Fill the tableau with the data
    
    // fill with 0s
    // we don't really need this but I don't like to see 1.0e-400 in the output
    for (int i = 0; i < m + 1; i++)
    {
        for (int j = 0; j < n + m + 1; j++)
        {
            T(i, j) = 0.0;
        }
    }

    // put b2 in the last column of T
    // notice the 0-indexing here!!!
    for (int i = 0; i < m + 1; i++)
    {
        T(i, n + m) += b2[i];
    }

    // put c in the first row of T
    for (int j = 0; j < n; j++)
    {
        T(0, j) += c[j];
    }

    // put A in the lower-left corner of T
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++) 
        {
            T(i + 1, j) += A(i, j);
        }
    }

    // fill the diagonal of non-basic variables with 1s (for the identity matrix)
    for (int i = 0; i < m; i++)
    {
        T(i + 1, n + i) += 1.0;
    }

    // Print the tableau
    cout << "Initial tableau:" << endl;
    
    // this won't work
    // it prints a funky 0x556da90020e8 or similar
    // cout << T << endl;

    // this will work
    // it prints the matrix in a nice format
    for (int i = 0; i < m + 1; i++)
    {
        for (int j = 0; j < n + m + 1; j++)
        {
            // cout << T(i, j) << " ";
            // if positive, print a space before or it will look misaligned
            if (T(i, j) >= 0.0)
            {
                cout << " ";
            }
            cout << T(i, j) << " ";
        }
        cout << endl;
    }

    // STEP 1 OF THE SIMPLEX

    // Cost criteria
    
    // start with the first element in the first row (arbitrary)
    double min_c_j = T(0, 0);

    // replace min_c_j with the smallest number in the first row
    for (int j = 0; j < n + m; j++)
    {
        if (T(0, j) < min_c_j)
        {
            min_c_j = T(0, j);
        }
    }

    // Print the minimum cost
    cout << "Minimum cost: " << min_c_j << endl;

    // Find the pivot column = Find the most negative entry in the first row
    int pivot_col = 0;
    for (int j = 0; j < n + m; j++)
    {
        if (T(0, j) < T(0, pivot_col))
        {
            pivot_col = j;
        }
    }

    // Find the pivow row = Find the row with the smallest positive ratio
    // again, 0-indexing
    int pivot_row = 1;
    for (int i = 1; i < m + 1; i++)
    {
        if (T(i, n + m) / T(i, pivot_col) < T(pivot_row, n + m) / T(pivot_row, pivot_col))
        {
            // here we need a 2nd if as this can take a_rs < 0 for the pivot
            // pivot_row = i;

            // if the ratio is positive, then we can use it
            if (T(i, n + m) / T(i, pivot_col) > 0) {
                pivot_row = i;
            }
        }
    }

    // Print the pivot row and column
    cout << "Pivot row: " << pivot_row << endl;
    cout << "Pivot column: " << pivot_col + 1 << endl;

    // Get the pivot element
    double pivot = T(pivot_row, pivot_col);

    // Divide the pivot row by the pivot element
    for (int j = 0; j < n + m + 1; j++)
    {
        T(pivot_row, j) /= pivot;
    }

    // Subtract multiples of the pivot row from each row to make all other entries in the pivot column zero
    for (int i = 0; i < m + 1; i++)
    {
        if (i != pivot_row)
        {
            double ratio = T(i, pivot_col);
            for (int j = 0; j < n + m + 1; j++)
            {
                T(i, j) -= ratio * T(pivot_row, j);
            }
        }
    }

    // Print the new tableau
    cout << "====\nNew tableau:" << endl;
    for (int i = 0; i < m + 1; i++)
    {
        for (int j = 0; j < n + m + 1; j++)
        {
            if (T(i, j) >= 0.0)
            {
                cout << " ";
            }
            cout << T(i, j) << " ";
        }
        cout << endl;
    }

    // Cost criteria

    // start with the first element in the first row (arbitrary)
    min_c_j = T(0, 0);

    // replace min_c_j with the smallest number in the first row
    for (int j = 0; j < n + m; j++)
    {
        if (T(0, j) < min_c_j)
        {
            min_c_j = T(0, j);
        }
    }

    // Print success message and stop if the minimum cost is non-negative
    if (min_c_j >= 0)
    {
        cout << "Optimal solution found in 1 step!" << endl;        
    }

    // Print the minimum cost
    cout << "Minimum cost: " << min_c_j << endl;

    // STEP 2, 3, ... OF THE SIMPLEX
    
    // Repeat until the first row contains no negative numbers

    // add a counter to keep track of the number of iterations
    // we previously completed 1 iteration
    int iter_count = 1;

    // the computational complexity of this algorithm is O(mn^2)
    // so we can use the dimensions of the tableau to set an upper bound on the number of iterations
    double iter_bound = m * n * n;

    while (min_c_j < 0)
    {
        if (iter_count > iter_bound)
        {
            stop("too many iterations");
        }

        // Find the pivot column = Find the most negative entry in the first row
        pivot_col = 0;
        for (int j = 0; j < n + m; j++)
        {
            if (T(0, j) < T(0, pivot_col))
            {
                pivot_col = j;
            }
        }

        // Find the pivow row = Find the row with the smallest positive ratio
        pivot_row = 1;
        for (int i = 1; i < m + 1; i++)
        {
            if (T(i, n + m) / T(i, pivot_col) < T(pivot_row, n + m) / T(pivot_row, pivot_col))
            {
                if (T(i, n + m) / T(i, pivot_col) > 0) {
                    pivot_row = i;
                }
            }
        }

        // Print the pivot row and column
        cout << "Pivot row: " << pivot_row << endl;
        cout << "Pivot column: " << pivot_col + 1 << endl;

        // Get the pivot element
        pivot = T(pivot_row, pivot_col);

        // Divide the pivot row by the pivot element
        for (int j = 0; j < n + m + 1; j++)
        {
            T(pivot_row, j) /= pivot;
        }

        // Subtract multiples of the pivot row from each row to make all other entries in the pivot column zero
        for (int i = 0; i < m + 1; i++)
        {
            if (i != pivot_row)
            {
                double ratio = T(i, pivot_col);
                for (int j = 0; j < n + m + 1; j++)
                {
                    T(i, j) -= ratio * T(pivot_row, j);
                }
            }
        }

        // Print the new tableau
        cout << "====\nNew tableau:" << endl;
        for (int i = 0; i < m + 1; i++)
        {
            for (int j = 0; j < n + m + 1; j++)
            {
                if (T(i, j) >= 0.0)
                {
                    cout << " ";
                }
                cout << T(i, j) << " ";
            }
            cout << endl;
        }
        
        // Cost criteria
    
        // start with the first element in the first row (arbitrary)
        min_c_j = T(0, 0);

        // replace min_c_j with the smallest number in the first row
        for (int j = 0; j < n + m; j++)
        {
            if (T(0, j) < min_c_j)
            {
                min_c_j = T(0, j);
            }
        }

        // Print the minimum cost
        cout << "Minimum cost: " << min_c_j << endl;

        // Increment the counter
        iter_count++;

        // Print success message and stop if the minimum cost is non-negative
        if (min_c_j >= 0)
        {
            cout << "Optimal solution found in " << iter_count << " steps !" << endl;
            break;
        }
    }
}
