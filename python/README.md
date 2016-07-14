Usage
----

作業フォルダに以下3つのファイルをコピーします。

ラッパーファイル
Windows: `C:\ProgramFiles(x86)\V-REP3\V-REP_PRO_EDU\programming\remoteApiBindings\python\python`  
Ubuntu: `$HOME/V-REP3/programming/remoteApiBindings/python/python`  
以下の
* `vrep.py`
* `vrepConst.py`

DLLファイル
Windows: `C:\ProgramFiles(x86)\V-REP3\V-REP_PRO_EDU\programming\remoteApiBindings\lib\lib`  
Ubuntu: `$HOME/V-REP3/programming\remoteApiBindings/lib/lib/'  
以下の32bitか64bitかは自分のPythonの環境に合わせて…
* Windows: `remoteApi.dll`
* Ubuntu: `remoteApi.so`

シミュレータを再生後、main.pyを実行

Memo
----
PotentiometerのTorqueをコードで変更すると振子が止まらない

