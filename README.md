Web application written in C++ and [Crow](https://github.com/ipkn/crow) web micro-framework for learning purposes.
Based on the LinkedIn Learning course "Web Servers and APIs using C++" by Troy Miles.

## Development and Installation instructions

Make sure you have [Docker]() installed.

### Basic Docker image
Build the base docker image that includes the basic dependencies:

```sh
cd cppbox
docker build -t cppbox .
```
This builds the docker image based on `cppbox/Dockerfile` and calls it `cppbox`.

### Compile the app

Run a docker container from the image created in the previous step so that you can
compile the app. Map the project root to the `/usr/src/cppweb` path in your container:

```sh
docker run -v /path/to/project:/usr/src/cppweb -ti cppbox:latest bash
```
Once inside the container, create a `build` folder under `hello_crow` directory
and run the build commands:

```sh
# inside the container
cd /usr/src/cppweb/hello_crow
# create the build folder
mkdir build
cd build
# create Makefile
cmake ..
# compile the app
make
```
Upon successful completion, this will create a `hello_crow` binary inside the `build` directory.
So the full absolute path inside the container is `/usr/src/cppweb/hello_crow/build/hello_crow`.

###  Create an image for the built app

Now that the app has been compiled, you can create a new image for the compiled app.

First let's run a container from our original image:
```sh
docker run -ti cppbox:latest bash
```
Note that we have not mapped a volume to the container.

Next, let's copy the project from the host system to the container.
We'll need to obtain the ID of the container we've just launched. In
a different terminal window, run:
```sh
docker ps
```
This will list available containers. Copy the ID of the container based on `cppbox:latest`.
Then copy the project directory from the host to the container:
```sh
docker cp /path/to/project containerId:/usr/src/cppweb
```
where `containerId` is the ID we got from the previous `docker ps` command.

Now that the container contains the project code and binary, we can create an image based
on the container by running the following command using the same container ID:
```sh
docker commit containerId hello_crow:latest
```
This creates the image from the container and calls it `hello_crow`.

### Create image to make deployment easier

Next we'll create another based on the image we created in the previous step.
This final image sets the `build` directory as the working directory and
starts the app automatically when the container is launched.

To create the image, we navigate to the `hello_crow` directory in our host and build the image based
on the `hello_crow/Dockerfile`:
```sh
cd /path/to/project/hello_crow
docker build -t hello_crow .
```

### Run the app locally

The following command will run the app on port 8080.
```sh
docker run -p 8080:8080 -e PORT=8080 hello_crow:latest
```

If you're still developing the app, you can still project directory as a volume on the
container:
```sh
docker run -v /path/to/project:/usr/src/cppweb -p 8080:8080 -e PORT=8080 hello_crow:latest
```

### Deployment to Heroku

Assuming you already have the Heroku CLI installed and authenticated on your machine,
the following commands will deploy the image created in the previous step on Heroku:

```sh
cd /path/to/project/hello_crow

heroku container:login

# replace app-name with the name of the app on Heroku
heroku create app-name

heroku container:push web -a app-name

heroku container:release -a app-name

# opens the app in a browser
heroku open -a app-name
```

