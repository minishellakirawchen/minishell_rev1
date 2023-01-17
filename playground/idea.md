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
