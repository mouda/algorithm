read_graph -d inputs/dg6.dot
dot -Tpng inputs/dg6.dot -o outputs/dg6.png
display outputs/dg6.png &
write_max_flow -s v0 -t v5 -o outputs/dg6_mf.dot
dot -Tpng outputs/dg6_mf.dot -o outputs/dg6_mf.png
display outputs/dg6_mf.png &
is_flow -i outputs/dg6_mf.dot
exit