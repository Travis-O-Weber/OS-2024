/******************************************************************************

                            Online Java Compiler.
                Code, Compile, Run and Debug java program online.
Write your code in this editor and press "Run" button to execute it.

*******************************************************************************/
public class MultithreadedSortingApplication {

    // Method for generating an array of double values
    static double[] generateArray(int n) {
        double[] array = new double[n];
        for (int i = 0; i < n; i++) {
            array[i] = 1.0 + Math.random() * 999.0; // Values between 1.0 and 1000.0
        }
        return array;
    }

    // Sorting_avg thread implementation
    static class Sorting_avg implements Runnable {
        private double[] subArray;
        private double average;

        public Sorting_avg(double[] subArray) {
            this.subArray = subArray;
        }

        public void run() {
            insertionSort(this.subArray, 0, this.subArray.length);
            this.average = calculateAverage(this.subArray);
        }

        public double getAverage() {
            return this.average;
        }
    }

    // Merging_avg thread implementation
    static class Merging_avg implements Runnable {
        private double[] firstSubArray, secondSubArray, mergedArray;
        private double overallAverage;

        public Merging_avg(double[] firstSubArray, double[] secondSubArray) {
            this.firstSubArray = firstSubArray;
            this.secondSubArray = secondSubArray;
        }

        public void run() {
            // Merge arrays and compute overall average
            this.mergedArray = mergeArrays(this.firstSubArray, this.secondSubArray);
            this.overallAverage = (calculateAverage(this.firstSubArray) + calculateAverage(this.secondSubArray)) / 2;
        }

        public double[] getMergedArray() {
            return this.mergedArray;
        }

        public double getOverallAverage() {
            return this.overallAverage;
        }
    }

    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: java MultithreadedSortingApplication <array_size>");
            return;
        }

        int n = Integer.parseInt(args[0]);
        double[] A = generateArray(n);

        // Multithreaded sorting and merging
        long startTimeMultithreaded = System.nanoTime();
        Sorting_avg sorter1 = new Sorting_avg(java.util.Arrays.copyOfRange(A, 0, n / 2));
        Sorting_avg sorter2 = new Sorting_avg(java.util.Arrays.copyOfRange(A, n / 2, n));
        Thread t1 = new Thread(sorter1);
        Thread t2 = new Thread(sorter2);
        t1.start();
        t2.start();
        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Merging_avg merger = new Merging_avg(sorter1.subArray, sorter2.subArray);
        Thread t3 = new Thread(merger);
        t3.start();
        try {
            t3.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        long endTimeMultithreaded = System.nanoTime();
        System.out.printf("Multithreaded sorting and merging took %.6f seconds.\n", (endTimeMultithreaded - startTimeMultithreaded) / 1e9);

        // Reset array for single-threaded operation
        A = generateArray(n);

        // Single-threaded sorting and averaging
        long startTimeSingleThreaded = System.nanoTime();
        insertionSort(A, 0, A.length);
        double average = calculateAverage(A);
        long endTimeSingleThreaded = System.nanoTime();
        System.out.printf("Single-threaded sorting took %.6f seconds. Average: %.2f\n", (endTimeSingleThreaded - startTimeSingleThreaded) / 1e9, average);
    }

    // Helper method for insertion sort
    static void insertionSort(double[] array, int start, int end) {
        // Insertion sort algorithm implementation
    }

    // Helper method for calculating average
    static double calculateAverage(double[] array) {
        double sum = 0;
        for (double v : array) {
            sum += v;
        }
        return sum / array.length;
    }

    // Helper method for merging two sorted arrays
    static double[] mergeArrays(double[] firstSubArray, double[] secondSubArray) {
        // Merging algorithm implementation
        return new double[firstSubArray.length + secondSubArray.length]; // Placeholder
    }
}