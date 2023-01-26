
# ft_export

* exit or return の統一
  * C39 return (EXIT_FAILURE) 
  * C51 exit (EXIT_FAILURE)
  * C93 return (EXIT_FAILURE)
  * 個人的には、exitは避けて可能な限り呼び出し元へ戻りエラー処理をしたい 
    * return (MALLOC_ERROR) とか？
    * これは今対応する必要はないが

* ft_export()
  * C92 !cmdsをチェックするなら!*cmdsもあった方が良いかも？ 
  * C90はC93の後でも良いかも？ 
    * （呼び出し的にはありえないが）!info || !com の結果を受けてからe_infoをmallocした方が良いかも

* define_ley_value()
  * C19, 20 ssize_t -> size_t ?
    * C18 exit_statusは常にEXIT_SUCCESSなら変数で持たなくもて良いかも？ 
  * C24 key_signで見つけたptrは使わない？ 
  * C33の作業と共通化できそう 
```c
while (cmd[eq_idx] != '=')
    eq_idx++
    
if (!cmd[eq_idx])
{
        e_info->key_type = e_novalue;
		e_info->key = cmd;
		return (exit_status);
}


```
get_set_elem.c
* set_elem()
  * C52-55
    * info->envlist_headの判定なしに`ft_lstadd_back(&info->envlist_head, new_env_node);`でOK
    * lstadd_backが同様の処理を行なっている

* libftでnull-safetyな関数を作成しています
  * ft_strlen_ns
  * ft_strlen_ns
* ダブルフリー防止のため, free後はnullの代入をしておきたい（念のため）
* exit or return の統一(ex ft_export.c)
  * C39 return (EXIT_FAILURE)
  * C51 exit (EXIT_FAILURE)
  * C93 return (EXIT_FAILURE)
  * 個人的には、exitは避けて可能な限り呼び出し元へ戻りエラー処理をしたい
    * return (MALLOC_ERROR) とか？
    * これは今すぐに対応する必要はないが、今後意識




minishell.h
C87 tpyeinit -> typeinit ?
