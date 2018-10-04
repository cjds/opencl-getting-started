## Working with OpenCL

I'm running OpenCL on an Intel CPU inside a Docker given here https://github.com/chihchun/opencl-docker


### Steps to getting this installed

1. Run the Docker:

```
docker run  -it --device=/dev/dri chihchun/opencl-intel  bash
```

2. Install dependencies:
```
apt -y install git ocl-icd-opencl-dev g++
```

3. Clone:

```
git clone https://github.com/cjds/opencl-getting-started
```

### Running above code

Run `make` in the clone directory and then run `./program`
