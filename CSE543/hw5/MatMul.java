/** A matrix multiplication Java program.

	The Java program has been developed to be as-close or nearly
	identical to that of MatMul.cpp to compare/contrast the
	performance of C++ implementation versus Java implementations.

	NOTE: All effort has been made to minimize garbage collection
	overheads (otherwise the Java memory usage and runtime becomes
	much higher)
*/
public class MatMul {

	public static final int MATRIX_SIZE = 3000;
	public static final int REPETITIONS = 1;

	public static void initMatrix(int matrix[][]) {
		for(int row = 0; (row < MATRIX_SIZE); row++) {
			for(int col = 0; (col < MATRIX_SIZE); col++) {
				matrix[row][col] = row + col;
			}
		}
	}

	public static void multiply(final int matrix1[][], final int matrix2[][],
								int matrix3[][]) {
		for(int row = 0; (row < MATRIX_SIZE); row++) {
			for(int col = 0; (col < MATRIX_SIZE); col++) {
				int sum = 0;
				for(int i = 0; (i < MATRIX_SIZE); i++)  {
					sum += (matrix1[row][i] * matrix2[i][col]);
				}
				matrix3[row][col] = sum;
			}
		}
	}

	public static int sumDiag(int matrix1[][]) {
		int sum = 0;
		for(int i = 0; (i < MATRIX_SIZE); i++) {
			sum += matrix1[i][i];
		}
		return sum;
	}

	public static final int matrix1[][] = new int[MATRIX_SIZE][MATRIX_SIZE];
	public static final int matrix2[][] = new int[MATRIX_SIZE][MATRIX_SIZE];
	public static final int matrix3[][] = new int[MATRIX_SIZE][MATRIX_SIZE];

	public static void main(String args[]) {
		System.out.println("Running " + REPETITIONS + " repetitions of " +
						   MATRIX_SIZE + "x" + MATRIX_SIZE + " matrix " +
						   "multiplication.");
		initMatrix(matrix1);
		initMatrix(matrix2);
		for(int i = 0; (i < REPETITIONS); i++) {
			multiply(matrix1, matrix2, matrix3);
			int diagSum = sumDiag(matrix3);
			System.out.println("Diag sum " + diagSum);
		}
	}
}
