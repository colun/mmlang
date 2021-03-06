MM Language
===========

MM Languageは、競技プログラミングのために開発中のプログラミング言語です。
どんなことが可能なのかは、examplesおよびexamples_outputsを参照ください。

まだ開発中の言語であるため、諸々不備があり、コンパイルエラーの行数表示さえまともに出せない状態です。
慣れて使うしかない様なヒドい状態ですので、ご容赦ください。

ライセンス
----------

ライセンスに関しては、以下の記事を参考にしながら準備中です。

https://kimiyuki.net/blog/2020/02/14/licenses-for-kyopro-libraries/

従って、現時点ではライセンスなしの状態になります。

MM Language プロジェクトでは、プロジェクトの様々な部分をカバーするいくつかのライセンスを検討中です。

 * MM Language コンパイラ（.pyなどのコード）については、MITライセンス
 * libフォルダ以下のC++の部分ライブラリ群およびexamplesフォルダ以下は、kimiyukiさんの記事にある様に、ライセンスなし、あるいは、下記の文の様な状態を目指します。

「このライブラリは、MIT License のもとで公開されている。ただし、競技プログラミングのためにこのライブラリの一部をジャッジサーバなどに送信するとき、著作権表示および許諾表示をmmlangによって生成されるコード内に自動的に埋め込まれる https://github.com/colun/mmlang へのリンクで代替することができる」

生成されるC++コードの著作権については、基本的に.m2ソースコードの著者へ帰属するはずだと見ていますが、埋め込まれるライブラリの著作権については、ライブラリの著作権者に権利が帰属するはずとの認識です。
この辺はHaxeなどのライセンスで前例があり、kimiyukiさんの要求を満たした上で、かつ言語部分とライブラリとで別々のライセンスであることをどう表記するのか、そして英語の問題をどう解決するのかなどが問題となります。

誰か、詳しい方、力を貸して頂けると嬉しいのですが……。

kimiyukiさん解釈だと現状のライセンスなしの状態であれば競プロ利用が文化的に困らないであろう推定判断が可能な様なので、ライセンスなしのまま長期間放置される可能性があります。

使い方
------

python3のインストールおよび、 `python3 -m pip install lark-parser` などでのlark-parserのインストールが必要となります。
mmにパスを通して、任意の場所から起動してください。以下の様なUSAGEが表示されます。

    USAGE: mm [--output OUTPUT] [--run] TARGET
      --output OUTPUT : output file or folder(.cpp files)
      --run           : run mode
      TARGET          : target file or folder(.m2 files)

TARGETのみの指定の場合、TARGETは.m2ファイルである必要があり、生成されたC++コードは標準出力に表示されます。
--outputを指定した場合、標準出力の代わりに、OUTPUTへと生成されたC++コードが保存されます。

TARGETがフォルダのとき、OUTPUTもフォルダである必要があります。その場合、再帰的に.m2ファイルを探し出し、OUTPUTの対応する場所へと生成されたC++コードを保存します。

すなわち、examples_outputsは、以下の様なコマンドにて一括コンパイルしています。

ユーザー自作のC++ライブラリの追加について
---------------------------

libフォルダおよびsrc/lib.pyを編集することで、任意のC++ライブラリがある程度使える様になります。
AtCoderLibraryの一部のライブラリの様にtemplateに独自クラスや独自関数を渡さなければならない様なライブラリには対応できていない気がします。

libフォルダ以下は、.cppファイルへと埋め込まれる際、自動的に圧縮されます。
デバッグなどの事情で圧縮されない方がよいものについては、 `src/lib.py` の `nocompress` にて、その.hファイルを指定してください。
