echo "*******************  brute force ********************";
#./01kanpsack -BF  ../inputs/n5S11.in  ../outputs/n5S11.out
./01kanpsack -BF ../inputs/n10s100.in ../outputs/n10s100.out
echo "";
echo "*******************  Greedy      ********************";
#./01kanpsack -GD  ../inputs/n5S11.in  ../outputs/n5S11.out
./01kanpsack -GD ../inputs/n10s100.in ../outputs/n10s100.out
echo "";
echo "*******************  Recursive   ********************";
#./01kanpsack -RC  ../inputs/n5S11.in  ../outputs/n5S11.out
./01kanpsack -RC ../inputs/n10s100.in ../outputs/n10s100.out
echo "";
echo "***************  Dynamic programming ****************";
#./01kanpsack -DP  ../inputs/n5S11.in  ../outputs/n5S11.out
./01kanpsack -DP ../inputs/n10s100.in ../outputs/n10s100.out
echo "";
