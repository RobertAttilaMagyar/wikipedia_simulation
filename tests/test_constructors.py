from wikipedia_module import Network

def test_empty_constructor():
    net = Network(2)
    assert net.num_editors == 0
    assert net.num_articles == 0

def test_parametrized_constructor():
    net = Network(10, 2, 3)

    assert net.num_editors == 2
    assert net.num_articles == 3

    

def test_add_nodes():
    net = Network(5)
    net.add_article()
    
    assert net.num_articles == 1
    assert net.num_editors == 0

    net.add_editor()
    assert net.num_editors == 1
    assert net.num_articles == 1