# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.154.0/containers/cpp/.devcontainer/base.Dockerfile

# [Choice] Debian / Ubuntu version: debian-10, debian-9, ubuntu-20.04, ubuntu-18.04
ARG VARIANT="buster"
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

RUN apt-get update && apt-get install -y clang-format && rm -rf /var/lib/apt/lists/*