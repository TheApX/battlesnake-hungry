for i in \
    bfs_tree_order \
    bfs_graph_order \
    bfs_intermediate_state \
; do
    dot -Tsvg -odocs/$i.svg docs/graphs/$i.dot
done

for i in \
    board_graph \
    board_graph_occupied \
; do
    fdp -Tsvg -odocs/$i.svg docs/graphs/$i.dot
done
