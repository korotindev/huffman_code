```
$> git clone git@github.com:korotindev/huffman_code.git
$> docker-compose build
$> docker-compose run --rm app bash
vscode ➜ / $ cd workspace/
vscode ➜ / $ make
vscode ➜ / $ make test
```

Or you can use [Remote-Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers ) extension in VSCode (see `.devcontainer` folder)

`make` will produce 3 files: 
- `build/encode` see help (just run it)
- `build/decode` see help (just run it)
- `build/test` runs by `make test`

Note: I've tested it only inside docker but you can try to build and run it locally :) 