

# wildcard
Use wildcards in arguments in the current working directory.

current working directoryにあるfile, directoryを取得
一致するものを引数とする

token[*] -> token[file1]->[file2]->...へchangeする


要チェック
* 展開を含むケースも同様

bash-3.2 0 $ export a1="echo *"
bash-3.2 0 $ $a1
in1 in2 ngfile out


'*'の展開はexpansionに組み込もう

取り組みの流れ

* current working directoryのファイル一覧を取得
* "*"と一致するものを出力
 * "**...*" は"*"と同様
 * "*a*"の前後*は別の文字列でもOK
 * 一致するものがなければそのまま出力
 * *はから文字以上の文字列, "abc*"や"*abc*","*a*b*c*" で "abc"がヒットする
 * むずくね？

* 前から貪欲に探していけば良さそう->複雑なケースでWA、dpでやった

```shell
bash-3.2 0 $ ls
in1     in2     ngfile  out

bash-3.2 0 $ echo *
in1 in2 ngfile out

bash-3.2 0 $ cat *
in1
in2
cat: ngfile: Permission denied
hoge


bash-3.2 1 $ ls *
in1     in2     ngfile  out

bash-3.2 0 $ ls in*
in1     in2

bash-3.2 0 $ ls in* ou*
in1     in2     out


bash-3.2 0 $ echo "*"
*
bash-3.2 0 $ echo '*'
*

bash-3.2 0 $ echo ****a
****a

```
リダイレクトもwildcard展開する








export b=*は展開されない
ここの除外がむずすぎる





permission 000
```shell

bash-3.2-2 0 $ chmod 000 ../playground/
bash-3.2-2 0 $ ls
ls: .: Permission denied

bash-3.2-2 1 $ ls *
ls: *: Permission denied

bash-3.2-2 1 $ cat *
cat: *: Permission denied

bash-3.2-2 1 $ echo *
*

bash-3.2-2 0 $ *
bash: *: command not found

```




# Expansion

```shell

bash-3.2$ export d1="echo -n hello world"
bash-3.2$ $d1       # hello worldbash-3.2$  promptにecho~を入力した時と同じ挙動

bash-3.2$ export d3=$d1$d2
bash-3.2$ echo $d3  # echo -n hello worldls -l > outfile
bash-3.2$ $d3       # hello worldls -l > outfilebash-3.2$ 

bash-3.2$ echo $d3  # echo -n hello worldls -l > outfile

bash-3.2$ export d5="echo hello world | $d3"
bash-3.2$ echo $d5  # echo hello world | echo -n hello worldls -l > outfile
bash-3.2$ $d5       # hello world | echo -n hello worldls -l > outfile
# echoコマンドの引数に"hello world | echo -n hello worldls -l > outfile"があったときの挙動
# expansion->space splitで良さそう
# "hoge"huga問題はある？

bash-3.2$ export e1="echo hello"world'huga'
bash-3.2$ echo $e1  # echo helloworldhuga
bash-3.2$ export e2="test   a  $e1'hogehoge'"
bash-3.2$ echo $e2  # test a echo helloworldhuga'hogehoge'

bash-3.2$ env
e1=echo helloworldhug
e2=test   a  echo helloworldhuga'hogehoge'  # expansion後にspace splitしてtokenizeされている


bash-3.2$ export a=b
bash-3.2$ export c=d$a
bash-3.2$ export e=c"$a"
bash-3.2$ env
a=b
c=db
e=cb

bash-3.2$ echo $a$a$a   # bbb
bash-3.2$ echo $a_$a$a  # bb
bash-3.2$ echo $a?$a$a  # b?bb

bash-3.2$ export x1=ech"o hello"
bash-3.2$ $x1           # hello

bash-3.2$ export x2=outfile
bash-3.2$ echo hoge > $x2 #echo hoge > outfile
bash-3.2$ export x3=">outfile"
bash-3.2$ echo hoge $x3   #echo "hoge >outfile"

bash-3.2$ export x4="test test"
bash-3.2$ echo hoge > $x4      
bash: $x4: ambiguous redirect

bash-3.2$ echo hoge > "test test" <-"test test"
bash-3.2$ cat "test test"
hoge
bash-3.2$ echo hoge > a b        
bash-3.2$ cat a
hoge b

bash-3.2$ echo hoge>a b c >d
bash-3.2$ ls
a               d               outfile         test test       test1           test2
bash-3.2$ cat a
bash-3.2$ cat d
hoge b c
bash-3.2$ echo hoge>a b c   
bash-3.2$ ls  
a               d               outfile         test test       test1           test2
bash-3.2$ cat a
hoge b c
bash-3.2$ 

// same as echo b c > a
```

* $を探す
* $key_candidate を探す
  * delimまでがkey候補
    * "$? "
* $key1, key1=key2, key3=key3,...だった時は？


```shell
bash-3.2$ 
bash-3.2$ export key1=key2
bash-3.2$ export key3=$key1               # $key1=key2
bash-3.2$ export key4=$key3$key1          # $key3=$key1=key2, $key1=key2->$key3$key1=key2key2
bash-3.2$ export key5=$key4"$key4"'$key4' # key4=key2key2, "$key4"=key2key2, '$key4'='$key4'
bash-3.2$ env    
key1=key2
key3=key2
key4=key2key2
key5=key2key2key2key2$key4
bash-3.2$ 

bash-3.2$ export a1=A1 && echo $a1 && export a2=$a1 && export a1=AA1; echo $a1 ;echo $a2
A1  # a1=A1
AA1 # a1=AA1
A1  # a2=$a1=A1(when assign a2=$a1, $a1=A1)


bash-3.2$ export z1=">"
bash-3.2$ echo hello $z1 testfile
hello > testfile


```

* redirect_listはre tokenizeしない, quote removalのみ
* 
```shell

bash-3.2$ export a2="cho hello        world"
bash-3.2$ echo $a2
cho hello world
bash-3.2$ e$a2
hello world
bash-3.2$

e$key = echo

bash-3.2$ e"cho" hello	//hello				-> {"echo", "hello"}
bash-3.2$ e"cho hoge"	//bash: echo hoge: command not found				-> {"echo hoge"}

bash-3.2$ echo $a1		//hoge huga			-> {"echo", "[hoge huga]"}
bash-3.2$ e"cho $a1"		//bash: echo hoge    huga : command not found	-> "e[cho $a1]={"echo hoge   huga"}

bash-3.2$ export a2="cho hello        world"
bash-3.2$ echo $a2		//cho hello world	-> {"echo", "[cho hello        world]"}

bash-3.2$ export a3="echo hello world"
bash-3.2$ $a3				//hello world		-> {"echo", "hello", "world"}

bash-3.2$ export a3="echo hello world"
bash-3.2$ echo $a3		//hello world
bash-3.2$ echo "$a3"		//hello      world

bash-3.2$ $a3				//bash: hello: command not found
bash-3.2$ "$a3"			//bash: hello      world: command not found

$key  :not split
"$key":split



<< include quote('")  heredoc is not expanded
<< $key   -> eofは$key, heredoc $key is expanded
<< "$key" -> eofは$key, heredoc is not expanded

bash 0$ export key=hoge
bash 0$ <<end"end"
> $key
> "$key"
> hoge
> endend

bash 0$ cat <<end""
> $key
> "$key"
> end
$key
"$key"

bash 0$ cat <<$key
> "$key"
> '$key'
> $key
"hoge"
'hoge'

bash 0$ cat << "$key"
> "$key"
> '$key'
> $key
"$key"
'$key'

bash 0$ cat << '$key'
> "$key"
> '$key'
> $key
"$key"
'$key'

bash 0$ 
bash 0$ 
bash 0$ cat << ""
> $key
> "$key"
> '$key'
> 
$key
"$key"
'$key'

bash 0$ cat << ''
> $key
> '$key'
> "$key"
> 
$key
'$key'
"$key"


```

```shell

test here_doc1 <<end<<"end" <<'end' &&
test here_doc2 <<"end  "hello <<'end  'hello << "hello "world'  good''morning'  &&
test here_doc3 <<$key <<"$key" <<'$key' &&
test here_doc4 << "$key  "end  << "
$key"'  $key'$key$key ;
 
test here_doc1 <<end<<"end" <<'end' &&test here_doc2 <<"end  "hello <<'end  'hello << "hello "world'  good''morning'  &&test here_doc3 <<$key <<"$key" <<'$key' &&test here_doc4 << "$key  "end  << "$key"'  $key'$key$key ;

[#DEBUG print] expansion 
  [pipeline] subshell  :
             commands  :[test]w ,[here_doc1]w 
             redirect  :<<[heredoc:end]y, <<[heredoc:end]n, <<[heredoc:end]n
    && operator
  [pipeline] subshell  :
             commands  :[test]w ,[here_doc2]w 
             redirect  :<<[heredoc:end  hello]n, <<[heredoc:end  hello]n, <<[heredoc:hello world  goodmorning]n
    && operator
  [pipeline] subshell  :
             commands  :[test]w ,[here_doc3]w 
             redirect  :<<[heredoc:$key]y, <<[heredoc:$key]n, <<[heredoc:$key]n
    && operator
  [pipeline] subshell  :
             commands  :[test]w ,[here_doc4]w 
             redirect  :<<[heredoc:$key  end]n, <<[heredoc:$key  $key$key$key]n


test in1 <end<"end" <'end' &&
test in2 <"end  "hello <'end  'hello < "hello "world'  good''morning'  &&
test in3 <$key <"$key" <'$key' &&
test in4 < "$key  "end  < "$key"'  $key'$key$key ;


test out1 >end>"end" >'end' &&
test out2 >"end  "hello >'end  'hello > "hello "world'  good''morning'  &&
test out3 >$key >"$key" >'$key' &&
test out4 > "$key  "end  > "$key"'  $key'$key$key ;



test append1 >>end>>"end" >>'end' &&
test append2 >>"end  "hello >>'end  'hello >> "hello "world'  good''morning'  &&
test append3 >>$key >>"$key" >>'$key' &&
test append4 >> "$key  "end  >> "$key"'  $key'$key$key ;




```



### heredocのexpansion
実行順は最優先, ;も貫通される
expansionの変数はどこの条件が使われる？
```shell
bash3.2 1 $ cat <<a; (cat <<b ; export a1=a2dayo && cat << c && export a1=a3dayo) ; cat <<d ;export a1=a4 && cat <<e

> this is a
> $a1 
> a

> this is b
> $a1
> b

> thisis c
> $a1
> c

> this is d
> $a1
> d

> thisis e
> $a1
> e

this is a
a4

this is b
a4

thisis c
a2dayo

this is d
a4

thisis e
a4

やばいやろこれ...

bash3.2 0 $ echo $a1;(echo $a1; export a1=a2dayo && echo $a1&& export a1=a3dayo); echo $a1;export a1=a4dayo&&echo $a1
a1
a1
a2dayo
a1
a4dayo

```

cat << a ;  (cat <<b  ; export a1=a2dayo &&   cat << c && export a1=a3dayo) ; cat <<d ;   export a1=a4      && cat <<e
       a4          a4                               a2dayo                          a4                              a4

echo $a1 ;  (echo $a1 ; export a1=a2dayo &&   echo $a1 && export a1=a3dayo) ; echo $a1 ;  export a1=a4dayo  && echo $a1
      a1           a1                              a2dayo                           a1                               a4dayo



Jan/29th
やり直し
展開内容は同じっぽい
heredocのexpansionはquote保持するので、
コマンド実行前にheredoc実行して
コマンド実行中にheredoc内のexpansionを実行する形かな
export a1=a2 && cat<<d のdがa1=a2になっているので、コマンド内のexpandと同様に実現できそう

heredocをpipelineからheredoc_listに移行しても、展開の整合性は保てる？
困る可能性があるのは、<<h1<in1 のように入力の優先順位がわからなくなること？
heredoc整形時にio_token_listにheredoc, redirect in/outを移動するか。めんど

```shell

// expand in heredoc
cat<<a; cat<<b && (cat<<c; export a1=a2&&cat<<d; cat<<e && export a1=a3; cat <<f && cat<<g; cat<<h); cat<<i && export a1=a4 && cat<<j; cat<<k;
    a1      a1        a1                    a2      a2                        a3       a3      a3       a1                        a4      a4
a1=a1 in a
a1=a1 in b
a1=a1 in c
a1=a2 in d
a1=a2 in e
a1=a3 in f
a1=a3 in g
a1=a3 in h
a1=a1 in i
a1=a4 in j
a1=a4 in k

// expand in echo
echo $a1; echo $a1 && (echo $a1; export a1=a2&&echo $a1; echo $a1 && export a1=a3; echo $a1 && echo $a1; echo $a1); echo $a1 && export a1=a4 && echo $a1; echo $a1;
      a1a       a1           a1                     a2          a2                       a3          a3        a3        a1                           a4        a4
a1
a1
a1
a2
a2
a3
a3
a3
a1
a4
a4

// expand in heredoc
bash3.2 0 $ cat <<end
> "$a1"
> $a1=$a1_hoge 
> "'$a1'"
> '"$a1"'
> end
"a4"
a4= 
"'a4'"
'"a4"'


```




































exportもどこかのタイミングでexpansionが必要か？

<br>
<br>
<br>

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
        [ ; ]                null 
     ┏━━━━┻━━━━┓       
  [cmd1]     [ | ]               
         ┏━━━━━┻━━━━━━┓
       [cmd2]       [cmd3]

cmdi: list
* cmd1 = echo -> hello -> world -> NULL
* cmd2 = cat -> Makefile -> NULL
* cmd3 = grep -> a

struct t_exec_list
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



Jan/19th update

bash-3.2$ ./minishell 
minishell $> echo "hello world"'<connect hoga' |cat -e Makefile>out>out>out>>append'hoge'"huga";echo;;echo||
#input          :[echo "hello world"'<connect hoga' |cat -e Makefile>out>out>out>>append'hoge'"huga";echo;;echo||]
#split space    :[echo],["hello world"]=['<connect hoga'],[|cat],[-e],[Makefile>out>out>out>>append]=['hoge']=["huga"]=[;echo;;echo||]
#split opes     :[echo],["hello world"]=['<connect hoga'],[|],[cat],[-e],[Makefile]=[>],[out],[>],[out],[>],[out],[>>],[append],['hoge']=["huga"]=[;],[echo],[;;],[echo],[||]


minishell $> echo hello | echo "hello "'world|<>' &&(cat<in<<heredoc>>out | grep a) ||test>out|cat -e hoge
#input          :[echo hello | echo "hello "'world|<>' &&(cat<in<<heredoc>>out | grep a) ||test>out|cat -e hoge]
#split space    :[echo(init)],[hello(init)],[|(init)],[echo(init)],["hello "(init)]=['world|<>'(init)],[&&(cat<in<<heredoc>>out(init)],[|(init)],[grep(init)],[a)(init)],[||test>out|cat(init)],[-e(init)],[hoge(init)]
#split opes     :[echo(init)],[hello(init)],[|(init)],[echo(init)],["hello "(init)]=['world|<>'(init)],[&&(init)],[((init)],[cat(init)],[<(init)],[in(init)],[<<(init)],[heredoc(init)],[>>(init)],[out(init)],[|(init)],[grep(init)],[a(init)],[)(init)],[||(init)],[test(init)],[>(init)],[out(init)],[|(init)],[cat(init)],[-e(init)],[hoge(init)]
#arranged       :[echo(word)],[hello(word)],[|(|)],[echo(word)],["hello "(word)]=['world|<>'(word)],[&&(&&)],[((()],[cat(word)],[<(<)],[in(file)],[<<(>>)],[heredoc(eof)],[>>(>)],[out(file)],[|(|)],[grep(word)],[a(word)],[)())],[||(||)],[test(word)],[>(<<)],[out(file)],[|(|)],[cat(word)],[-e(word)],[hoge(word)]


minishell $> echo "foo 'bar' test| &&<"|(cat <in && ls)&&out
#input          :[echo "foo 'bar' test| &&<"|(cat <in && ls)&&out]
#split space    :[echo(init)],["foo 'bar' test| &&<"(init)]=[|(cat(init)],[<in(init)],[&&(init)],[ls)&&out(init)]
#split opes     :[echo(init)],["foo 'bar' test| &&<"(init)]=[|(init)],[((init)],[cat(init)],[<(init)],[in(init)],[&&(init)],[ls(init)],[)(init)],[&&(init)],[out(init)]
#arranged       :[echo(word)],["foo 'bar' test| &&<"(word)],[|(|)],[((()],[cat(word)],[<(<)],[in(file)],[&&(&&)],[ls(word)],[)())],[&&(&&)],[out(word)]

```

export key1=$key2$key3　空白なしで結合される

input [hogehog   ]
              ^^^
これが""として残り、validate errorになる
"", ''も不要であれば削除する？


## sub-shellが難しい
```
((ls) && ls && (ls -l))


(a && (b || c | (d || e)) && f )

a && B && f
B = b || c | C
c = d || e

むずくね？
                           [sub-shell]
                  ┏━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┓
                [&&]                           [&&]
       ┏━━━━━━━━━━┻━━━━━━━━━━━┓             ┏━━━┻━━━━┓      
  [sub-shell]            [sub-shell]       [f]       null
  ┏━━━━┻━━━━┓           ┏━━━━━┻━━━━━━┓ 
[a]        null       [||]           null
                   ┏━━━┻━━━━┓
                  [b]      [|]
                        ┏━━━┻━━━━┓      
                       [c]   [sub-shell]
                             ┏━━━┻━━━━┓              
                            [d]       [e]
                                                   

まず、a && b && cを木構造で分割する方法が怪しい


        [ ; ]                null 
     ┏━━━━┻━━━━┓       
  [cmd1]     [ | ]               
         ┏━━━━━┻━━━━━━┓
       [cmd2]       [cmd3]

     




```
( = 1
) = -1
でcloseをチェック
0になった時までを区切りminishellに渡す

```
A ; B ; C    A->B->C
A    echo hello | grep a && (cd /bin && ls) || hoge

&&  cmd1=0 && cmd2
||  

A    a && b || c  a->b->c


a && b && c
   [&&]
 ┏━━━┻━━━━┓
[a]      [&&]
     ┏━━━┻━━━━┓
    [b]       [c]


$ a && (b && (c || d | (e || f)) && g)
$ X

X = a && Y1
Y1 = b && Y2 && g
Y2 = c || d | Y3
Y3 = e || f

#before
             [root]
               ┃
              [&&]
 ┏━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┓
[a]                         [sub-shell]
                  ┏━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━┓
                [&&]                           [&&]
       ┏━━━━━━━━━━┻━━━━━━━━━━━┓             ┏━━━┻━━━━┓      
  [sub-shell]            [sub-shell]       [g]       null
  ┏━━━━┻━━━━┓           ┏━━━━━┻━━━━━━┓ 
[b]        null       [||]           null
                   ┏━━━┻━━━━┓
                  [c]      [|]
                        ┏━━━┻━━━━┓      
                       [d]   [sub-shell]
                             ┏━━━┻━━━━┓              
                            [f]       [f]



#after
1)minishell root
    [X]
    [&&]
 ┏━━━┻━━━━┓
[a]       [Y1]
           ^minishell child-1

2)minishell child-1
    [Y1]
    [&&]
 ┏━━━┻━━━━┓
[b]      [&&]
      ┏━━━┻━━━━┓
    [Y2]       [g]
     ^minishell child-2

3)minishell child-2
    [Y2]
    [||]
 ┏━━━┻━━━━┓
[c]      [|]
      ┏━━━┻━━━━┓
    [d]       [Y3]
               ^minishell child-3

4)minishell child-3
    [Y3]
    [||]
 ┏━━━┻━━━━┓
[e]       [f]





# 23/Jan/20th
[0] a && (b && (c || d | (e || f)) && g)
*create root
    [root]
 ┏━━━━┻━━━━━┓
null       null                         

[1] a && (b && (c || d | (e || f)) && g)
      ^^ ope=&&

 *create node
         [&&]
     ┏━━━━┻━━━━━┓
    null        null                        

 *create leaf
      [command]
     ┏━━━━┻━━━━━┓
    [a]        null    

 *connect command leaf to operator node
              [&&]
          ┏━━━━┻━━━━━┓
      [command]     null                        
     ┏━━━━┻━━━━━┓
    [a]        null    
                   

 *connect to bottom operator node = root->prev
                 [root]
               ┏━━━━┻━━━━━┓
             [&&]       null
          ┏━━━━┻━━━━━┓
        [cmd]        null                        
     ┏━━━━┻━━━━━┓
    [a]        null                        

         0     1         2     2 1     0
[3] a && (b && (c || d | (e || f)) && g) = a && X1
         ^0 skip subshell             0^ ^end

 *create leaf

      [subshell-0]
     ┏━━━━┻━━━━━┓
    [X1]        null                        


 * connect to bottom operator node = &&->next 

                    [root]
                  ┏━━━━┻━━━━━┓
                 [&&]       null
          ┏━━━━━━━┻━━━━━━━┓
        [cmd]          [subshell-0]   
     ┏━━━━┻━━━━━┓     ┏━━━━┻━━━━━┓
    [a]        null  [X1]        null                       


実行時
exec X1:subshell -> pass to parsing func
## X1
[4] X1 = b && (c || d | (e || f)) && g

 *create root
 
        [root]
     ┏━━━━┻━━━━━┓
    null       null                         


[5] b && (c || d | (e || f)) && g
      ^^ ope=&&

 *create operator node

         [&&]
     ┏━━━━┻━━━━━┓
    null        null                        

 *create leaf

      [command]
     ┏━━━━┻━━━━━┓
    [b]        null    


 *connect command leaf to operator node

              [&&]
          ┏━━━━┻━━━━━┓
      [command]     null                        
     ┏━━━━┻━━━━━┓
    [b]        null    

                   
 *connect to bottom operator node = root->prev
                 [root]
               ┏━━━━┻━━━━━┓
             [&&]       null
          ┏━━━━┻━━━━━┓
        [cmd]        null                        
     ┏━━━━┻━━━━━┓
    [b]        null                        
    

         1         2     2 1         
[6] b && (c || d | (e || f)) && g = b && X2 && g
         ^ skip subshell   ^ ^^ope       ^^ ^^

 *create operator node
         [&&]
     ┏━━━━┻━━━━━┓
    null        null                        

 *create leaf
      [subshell-1]
     ┏━━━━┻━━━━━┓
    [X2]        null    

 *connect command leaf to operator node = &&->prev
              [&&]
          ┏━━━━┻━━━━━┓
   [subshell-1]     null                        
     ┏━━━━┻━━━━━┓
    [X2]        null    
   
 *connect to bottom root node = &&->next
 
                       [root]
                     ┏━━━━┻━━━━━┓
                   [&&]        null
          ┏━━━━━━━━━━┻━━━━━━━━━━━┓
        [cmd]                   [&&]                        
     ┏━━━━┻━━━━━┓           ┏━━━━┻━━━━━┓
    [b]        null   [subshell-1]     null
                       ┏━━━━┻━━━━━┓
                     [X2]        null    

[7] b && (c || d | (e || f)) && g = b && X2 && g
      0                      1         0    1 ^ ^end
                                               
 *create leaf

      [command]
     ┏━━━━┻━━━━━┓
    [g]        null


 *connect to &&-1 = &&1->next
 
                         [root]
                       ┏━━━━┻━━━━━┓
                    [&&-0]        null
          ┏━━━━━━━━━━━━┻━━━━━━━━━━━━┓
        [cmd]                      [&&-1]                        
     ┏━━━━┻━━━━━┓         ┏━━━━━━━━━┻━━━━━━━━┓
    [b]        null  [subshell-1]        [command]
                     ┏━━━━┻━━━━━┓       ┏━━━━┻━━━━━┓
                   [X2]        null    [g]        null


operator no必要？
それともbottomに結合すれば十分？
** A && B && Cの木構造はどうあるべき？

            [root]
         ┏━━━━┻━━━━━┓
       [&&-1]      null
  ┏━━━━━━┻━━━━━━┓
 [A]          [&&-2]                        
          ┏━━━━┻━━━━┓
         [B]       [C]

root->prev = pipeline_commands or operator
if (root->prev == operator)
 operator->prev = pipeline_commands
 operator->rigth = pipeline_commands or operator

左結合なので左側はpipeline
右側はpipeline or operator listのどちらか

root->next = NULL



root->prev = ope1(&&-1)
 ope1->prev = A
 ope1->next = ope2(&&-2)
  ope2->prev = B
  ope2->next = C

A && B && C && D


```

## EBNF(Extended Backus–Naur form)

### わかっていること
* control operator |, &&, ||, ;, ()
* operator <, >, <<, >>
* 構文解析、EBNFを使えると拡張性が高くなりそう
* 力技で優先順位ごとに分割し、リスト構造で持てばやりたいことは実行できそう
  * 力技の方法も構文木を分解しているだけっぽい

### わかったこと
* BNFによる表現方法
* &&,||,;は同列扱いで良さそう、前から実行して次への渡し方を分岐するだけっぽい
* BNFの使い方
  * BNFは定義であり、これをloadしてvalidationなどはしない
  * これをもとに実装していくということっぽい

### わかっていないこと
* 構文木の作成方法
* 優先順位に基づく実行方法


#### 文法の表現->生成規則'''
```
#  BNF->parsing

<digit> ::= 0|1|2|3|4|5|6|7|8|9

<number> ::= <digit>
           | <number> <digit>

<letter> ::= a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|
             A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z

<word> ::= <letter>
         | <word> <letter>

<word_list> ::= <word>
             |  <word_list> <word>

<assignment_word> ::= <word> '=' <word>
                | <word> '+=' <word>

<redirection> ::=  '>' <word>
                |  '<' <word>
                |  '>>' <word>
                |  '<<' <word>

<command_element> ::= <word>
                    | <assignment_word>
                    | <redirection>

<redirection_list> ::= <redirection>
                    | <redirection_list> <redirection>

<command> ::= <command_element>
            | <subshell>
            | <subshell> <redirection_list>
            

<pipeline_commands> ::=  <pipeline_commands> '|' <pipeline_commands>
                | <command>

<list> ::= <list> '&&' <list>
           |  <list> '||' <list>
           |  <list> ';' <list>
           |  <pipeline_commands>

<subshell> ::= '(' <list> ')'
 
 
 
 
 
 
<command_list>      ::= <command_element>
                      | <command_list>
                      | <subshell>
                      | <subshell> <redirect_list>

<subshell>          ::= '(' <operator_list> ')'

<pipeline_commands>          ::= <pipeline_commands> '|' <pipeline_commands>
                      | <command_list>

<operator_list>     ::= <operator_list> '&&' <operator_list>
                      | <operator_list> '||' <operator_list>
                      | <operator_list> ';' <operator_list>
                      | <pipeline_commands>
```

### tokenlist -> opetarot_list (= pipeline_commands && pipeline_commands || ...)

echo hello world | grep a && cat Makefile; echo hello; echo world
pipeline1                 && pipeline2   ; pipeline3 ; pipeline4

```c

t_exec_list *exec_list
{
    enum        node_kind; (head/operator/pipeline_commands)
    t_list      *token_list_head; (pipeline_commands; token1->token2->... for next process)
    t_list      *command_list_head; (create content in next process)
    t_exec_list *prev;
    t_exec_list *next;
}

```
head->pipeline1->operator(&&)->pipeline2->operator(;)->pipeline3->operator(;)->pipeline4

### pipeline_commands -> command_list (= commands | commands | ...)

pipeline_commands = command_list1 | command_list2 | ...
command_list1-> command_list2->...
"->(next)" means "|(pipe)"

```c

struct s_command_info
{
    char        *commands;      {"echo", "hello", NULL} for execve argument. expansion yet
    t_r_info    *redirect_info;  infomation of "infile, outfile, read, write, append, heredoc,.."
    t_list      *next;
}

```

1) echo hello world | grep a > out

2) cat Makefile > out | (echo hello; echo world && echo hoge) > out



### execute
```c


int execute_operator_list(t_exec_list *exec_list_head)
{
    t_exec_list *exec_node;
    t_pipe_line *pipe_node;
    t_node_kind operator_type;
    int         exit_status;
    
	if (!exec_list_head || !*exec_list_head)
		return (FAILURE);
	exec_node = *exec_list_head;
    operator_type = -1;
    while (exec_node)
    {
		// while (operator != ';') これが必要かも
		// 前から見ていくだけだったら、A || B && C; D |E のAで終了する可能性もある
		if (exec_node->kind == operator)
        {
            operator_type = get_operator_type(exec_node); &&, ||, ;
            exec_node = exec_node->next;
        }
        if (operator_type == ';' && !exec_node)
            break ;
        exit_status = exec_pipeline(exec_node); pipeline_commands->operator->pipeline_commands->operator,...
        
        if (operator_type == '&&' && exit_status == 0)
            break ;
        if (operator_type == '||' && exit_status != 0)
            break ;
        if (operator_type == ';')
            exit_status = 0; // TODO: check
        exec_node = exec_node->next;
    }
    return (exit_status);	
};


int execute_pipeline(t_list *command_list_head)
{
	
	t_list  *node;
	
	while (node)
	{
		if (subshell)
			execute_subshell(node);
		pipe
		dup
		fork
		exec
		
		node = node->next;
	}
	return (exit_status;)
}

```





