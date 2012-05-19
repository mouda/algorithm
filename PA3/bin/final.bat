read_graph ../inputs/gn4.dot
write_tree_dfs -s v0 -o ../outputs/gn4_dfs_n.dot
write_tree_bfs -s v0 -o ../outputs/gn4_bfs_n.dot
write_tree_mst -a prim -r v0 -o ../outputs/gn4_mst_n.dot

read_graph ../inputs/gn10.dot
write_tree_dfs -s v0 -o ../outputs/gn10_dfs_n.dot
write_tree_bfs -s v0 -o ../outputs/gn10_bfs_n.dot
write_tree_mst -a prim -r v0 -o ../outputs/gn10_mst_n.dot

read_graph ../inputs/gn100.dot
write_tree_dfs -s v0 -o ../outputs/gn100_dfs_n.dot
write_tree_bfs -s v0 -o ../outputs/gn100_bfs_n.dot
write_tree_mst -a prim -r v0 -o ../outputs/gn100_mst_n.dot

read_graph ../inputs/gn1000.dot
write_tree_dfs -s v0 -o ../outputs/gn1000_dfs_n.dot
write_tree_bfs -s v0 -o ../outputs/gn1000_bfs_n.dot
write_tree_mst -a prim -r v0 -o ../outputs/gn1000_mst_n.dot

read_graph ../inputs/gn10000.dot
write_tree_dfs -s v0 -o ../outputs/gn10000_dfs_n.dot
write_tree_bfs -s v0 -o ../outputs/gn10000_bfs_n.dot
write_tree_mst -a prim -r v0 -o ../outputs/gn10000_mst_n.dot

read_graph ../inputs/gn50000.dot
write_tree_dfs -s v0 -o ../outputs/gn50000_dfs_n.dot
write_tree_bfs -s v0 -o ../outputs/gn50000_bfs_n.dot
write_tree_mst -a prim -r v0 -o ../outputs/gn50000_mst_n.dot
