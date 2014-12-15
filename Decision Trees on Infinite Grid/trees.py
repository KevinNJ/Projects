__author__ = 'knelson'


transitions = {
    'L': ['U', 'D', 'L'],
    'R': ['U', 'D', 'R'],
    'U': ['L', 'R', 'U'],
    'D': ['L', 'R', 'D'],
    None: ['L', 'R', 'U', 'D']
}


class Node(object):
    def __init__(self):
        self.children = []
        self.decision = None


def make_tree(node, state):
    for d,v in state.iteritems():
        if d in transitions[node.decision] and v > 0:
            child = Node()
            child.decision = d
            new_state = state.copy()
            new_state[d] -= 1
            make_tree(child, new_state)
            node.children.append(child)


def list_paths(parent):
    if len(parent.children) == 0:
        return [[parent.decision]]
    else:
        paths = []
        for child in parent.children:
            paths.extend(list_paths(child))
        if parent.decision is not None:
            for path in paths:
                path.insert(0, parent.decision)
        return paths


def run_tuple(l,r,u,d):
    root = Node()
    make_tree(root, {'L':l, 'R':r, 'U':u, 'D':d})
    path_len = sum((l,r,u,d))
    paths = filter(lambda x: len(x) == path_len, list_paths(root))
    print 'Path Len, Number of paths (%s,%s)' % (path_len, len(paths))
    #print '\n'.join([', '.join(x) for x in paths])
    #print ''
    return root, paths


def make_graph(graph, root, root_node):
    for child in root.children:
        child_node = pydot.Node(get_next_id(), label=child.decision)
        dot_edge = pydot.Edge(root_node, child_node)
        graph.add_node(child_node)
        graph.add_edge(dot_edge)
        make_graph(graph, child, child_node)


def get_next_id():
    nid = get_next_id.id
    get_next_id.id += 1
    return nid
get_next_id.id = 1


if __name__ == '__main__':

    pathDic = {
        3: [(0,2,1,0)],
        5: [(1,3,1,0),(0,2,2,1)],
        7: [(2,4,1,0),(0,2,3,2),(1,3,2,1)]
    }

    import pydot

    for l,p in pathDic.iteritems():
        for tup in p:
            graph = pydot.Dot(graph_type='graph', dpi=300)
            root, paths = run_tuple(*tup)
            root_node = pydot.Node('X')
            make_graph(graph, root, root_node)

            graph.write_png('L%s_' % l + '-'.join(map(str, tup)) + '.png')