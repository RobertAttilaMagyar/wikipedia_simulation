from wikipedia_module import Network

def test_empty_constructor():
    net = Network(2)
    assert len(net.editor_ids) == 0
    assert len(net.article_ids) == 0

def test_parametrized_constructor():
    net = Network(10, 2, 3)

    assert len(net.editor_ids) == 2
    assert len(net.article_ids) == 3

    

def test_add_nodes():
    net = Network(5)
    net.add_article()
    
    assert len(net.article_ids) == 1
    assert len(net.editor_ids) == 0

    net.add_editor()
    assert len(net.editor_ids) == 1
    assert len(net.article_ids) == 1