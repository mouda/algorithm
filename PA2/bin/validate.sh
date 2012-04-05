echo "*******************  brute force ********************";
#./01knapsack -BF  ../inputs/n5S11.in  ../outputs/n5S11.out
./01knapsack -BF ../inputs/n10s100.in ../outputs/n10s100.out
echo "";
echo "*******************  Greedy      ********************";
#./01knapsack -GD  ../inputs/n5S11.in  ../outputs/n5S11.out
./01knapsack -GD ../inputs/n10s100.in ../outputs/n10s100.out
echo "";
echo "*******************  Recursive   ********************";
#./01knapsack -RC  ../inputs/n5S11.in  ../outputs/n5S11.out
./01knapsack -RC ../inputs/n10s100.in ../outputs/n10s100.out
echo "";
echo "***************  Dynamic programming ****************";
#./01knapsack -DP  ../inputs/n5S11.in  ../outputs/n5S11.out
./01knapsack -DP ../inputs/n10s100.in ../outputs/n10s100.out
echo "";
