echo hello world; cat Makefile | grep a

```
                     [ root ]
          ┏━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━┓
        [ ; ]                             [ | ] 
     ┏━━━━┻━━━━┓                     ┏━━━━━━┻━━━━━━━━━━━┓
  [echo]   ┏━━━┻━━━━┓            ┏━━━┻━━━┓          ┏━━━┻━━━┓
       [hello]  ┏━━━┻━━━┓      [cat] ┏━━━┻━━━┓   [grep] ┏━━━┻━━━┓
             [wold]    null     [Makefile]  null       [a]      null
```

```
$ echo hello world; cat Makefile | grep a
$ cmd1 ; cmd2 | cmd3

                [ root ]
          ┏━━━━━━━━┻━━━━━━━━━━┓
        [ ; ]               [ | ] 
     ┏━━━━┻━━━━┓        ┏━━━━━┻━━━━━━┓
  [cmd1]      null     [cmd2]       [cmd3]          

cmdi: list
* cmd1 = echo -> hello -> world -> NULL
* cmd2 = cat -> Makefile -> NULL
* cmd3 = grep -> a

struct t_tree
{
    type; root/ope/cmd
    ope; ;/&&/||/()/|
    cmd; *list_head
}

```


# Tokenize
```c

minishell
$ echo hello world"hoge" "hello'''""world"'huga''piyo'
input:  [echo hello world"hoge" "hello'''""world"'huga''piyo']
# [echo],[hello],[world"hoge"],["hello'''""world"'huga''piyo']

bash
$ echo hello world"hoge" "hello'''""world"'huga''piyo'
hello worldhoge hello'''worldhugapiyo


この段階で""split, 結合情報保持しておいた方が楽か?
次に任せる?
この段階で分けることにした

minishell $> echo hello"hoge"'huga' "test'piyo'" "" "   test   "
input:  [echo hello"hoge"'huga' "test'piyo'" "" "   test   "]
token list
# [echo],[hello]=["hoge"]=['huga'],["test'piyo'"],[""],["   test   "]

minishell $> echo hello world"hoge" "hello'''""world"'huga''piyo'
input:  [echo hello world"hoge" "hello'''""world"'huga''piyo']
token list
# [echo],[hello],[world]=["hoge"],["hello'''"]=["world"]=['huga']=['piyo']


```