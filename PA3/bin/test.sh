./graphlab -f validate > /dev/null 2>/dev/null
echo "*********** gn4 bfs **********";
diff ../outputs/gn4_bfs.dot ../outputs/gn4_bfs_result.dot 
echo "*********** gn4 dfs **********";
diff ../outputs/gn4_dfs.dot ../outputs/gn4_dfs_result.dot 
echo "*********** gn4 mst **********";
diff ../outputs/gn4_mst_p.dot ../outputs/gn4_mst_result.dot 
