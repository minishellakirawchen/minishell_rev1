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
```shell
#1
minishell $> echo hello"hoge"'huga' "test'piyo'" "" "   test   "
input:  [echo hello"hoge"'huga' "test'piyo'" "" "   test   "]
token list
 # [echo],[hello]=["hoge"]=['huga'],["test'piyo'"],[""],["   test   "]

bash-3.2$ echo hello"hoge"'huga' "test'piyo'" "" "   test   "
hellohogehuga test'piyo'     test 


#2
minishell $> echo hello world"hoge" "hello'''""world"'huga''piyo'
input:  [echo hello world"hoge" "hello'''""world"'huga''piyo']
token list
 # [echo],[hello],[world]=["hoge"],["hello'''"]=["world"]=['huga']=['piyo']

bash-3.2$ echo hello world"hoge" "hello'''""world"'huga''piyo'
hello worldhoge hello'''worldhugapiyo


#3
minishell $> echo "hoge'"''foo''''"huga'"
input:  [echo "hoge'"''foo''''"huga'"]
token list
 # [echo],["hoge'"]=['']=[foo]=['']=['']=["huga'"]

bash-3.2$  echo "hoge'"''foo''''"huga'"
hoge'foohuga'


```

## わからないこと
* 分割の順番、方法（space splitはできたが...）
* 前後結合情報の保持
* split -> expansionの実行順

## わかっていること
* $varの展開を早めに実施しないとまずい？
```shell

bash-3.2$ a1="echo hello"
bash-3.2$ a2="cat test1"
bash-3.2$ a3="ls -l"

bash-3.2$ a1
bash: a1: command not found

bash-3.2$ a2
bash: a2: command not found

bash-3.2$ a3
bash: a3: command not found

bash-3.2$ $a1
hello

bash-3.2$ $a2
test1

bash-3.2$ $a3
total 16
-rw-r--r--  1 akira  staff  6 Jan 18 21:08 test1
-rw-r--r--  1 akira  staff  6 Jan 18 21:08 test2

# $var1 $var2 .. 先頭のコマンドのみ有効
bash-3.2$ $a1 $a2 $a3
hello cat test1 ls -l

bash-3.2$ $a1$a2$a3
hellocat test1ls -l

# "$var"はone-wordと認識される
bash-3.2$ "$a1"
bash: echo hello: command not found

bash-3.2$ "$a2"
bash: cat test1: command not found

bash-3.2$ "$a3"
bash: ls -l: command not found


# 演算子有効
bash-3.2$ $a1 | $a2 | $a3
total 16
-rw-r--r--  1 akira  staff  6 Jan 18 21:08 test1
-rw-r--r--  1 akira  staff  6 Jan 18 21:08 test2

bash-3.2$ $a1 && $a2 && $a3
hello
test1
total 16
-rw-r--r--  1 akira  staff  6 Jan 18 21:08 test1
-rw-r--r--  1 akira  staff  6 Jan 18 21:08 test2

# コマンドは先頭の文字だけ、control operatorは有効にならない
bash-3.2$ a4="cat test1 | grep a"
bash-3.2$ $a4
test1

cat: |: No such file or directory
cat: grep: No such file or directory
cat: a: No such file or directory

# 入れ子 ""内に複数文字入れた場合と同じ
bash-3.2$ a5="$a1 ls"

bash-3.2$ a5
bash: a5: command not found

bash-3.2$ $a5
hello ls

bash-3.2$ echo $a5
echo hello ls


```
""内の環境変数を展開できるだけ展開すれば、あとは同様の挙動になりそう

```shell

bash-3.2$ echo $a1

bash-3.2$ a1="hello world"
bash-3.2$ a2=bye
bash-3.2$ echo "$a1"
hello world
bash-3.2$ echo a1
a1
bash-3.2$ export a1="hello world"
bash-3.2$ export a2=bye
bash-3.2$ export a3="good $a2"
bash-3.2$ echo $a4

bash-3.2$ echo $a3
good bye
bash-3.2$ export a4='good "$a2"' 
bash-3.2$ export a5='hoge "$a1"' 
bash-3.2$ echo $a4
good "$a2"
bash-3.2$ echo $a5
hoge "$a1"
bash-3.2$ 


```

```shell

bash-3.2$ export c1=hoge
bash-3.2$ export c2=$c1
bash-3.2$ export c3=$c2

bash-3.2$ echo $c3 $c2 $c1
hoge hoge hoge

bash-3.2$ export c1=fuga
bash-3.2$ echo $c3 $c2 $c1
hoge hoge fuga
```

* expansionは沼ではなさそう
* $key=valueに変えるだけ
* スペースがあるときはどうする？
* space splitして、1commandsとして分割すれば良さそう
* pipeは記号
```shell
bash-3.2$ export d1="echo -n hello world"
bash-3.2$ $d1
hello worldbash-3.2$ 

bash-3.2$ export d3=$d1$d2
bash-3.2$ export d3
bash-3.2$ export d3=$d1$d2
bash-3.2$ echo $d3
echo -n hello worldls -l > outfile
bash-3.2$ $d3
hello worldls -l > outfilebash-3.2$ 

bash-3.2$ echo $d3
echo -n hello worldls -l > outfile

bash-3.2$ export d5="echo hello world | $d3"
bash-3.2$ echo $d5
echo hello world | echo -n hello worldls -l > outfile
bash-3.2$ $d5
hello world | echo -n hello worldls -l > outfile
# echoコマンドの引数に"hello world | echo -n hello worldls -l > outfile"があったときの挙動
# expansion->space splitで良さそう
# "hoge"huga問題はある？

bash-3.2$ export e1="echo hello"world'huga'
bash-3.2$ echo $e1
echo helloworldhuga
bash-3.2$ export e2="test   a  $e1'hogehoge'"
bash-3.2$ echo $e2
test a echo helloworldhuga'hogehoge'


bash-3.2$ env
e1=echo helloworldhug
e2=test   a  echo helloworldhuga'hogehoge'



```

export key1=$key2$key3　空白なしで結合される





