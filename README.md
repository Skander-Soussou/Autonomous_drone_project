# Getting Started with Create React App

This project was bootstrapped with [Create React App](https://github.com/facebook/create-react-app).

### Table of contents
 - [Prerequisites](#prerequisites)
 - [Usage](#Usage)
   - [Running the server](#Running-the-server)
   - [Running the client](#Running-the-client)
 - [Available Scripts](#Available-Scripts)
 - [Testing](#Tests)

### Prerequisites
To build the project, you'll need to run yarn and flask (and an extension for handling Cross Origin Resource Sharing (CORS))

To download the dependencies for the server, in /server/bin, run the following commands :
```
$ pip install -r requirement.txt
```

Make sure that Node is installed first. To install `yarn`, run the following command in the client folder : 
```
$ npm install --global yarn
```

### Usage

#### Running the server

In `/server` folder, execute the commands : 

```
$ python3 server.py
```

### Running the client

To run the client, execute :
```
yarn start
```

## Available Scripts

In the `/client/` directory, you can run:

### `yarn start`
Runs the app in the development mode.\
Open [http://localhost:3000](http://localhost:3000) to view it in the browser.

The page will reload if you make edits.\
You will also see any lint errors in the console.

### `yarn test`

Launches the test runner in the interactive watch mode.\
See the section about [running tests](https://facebook.github.io/create-react-app/docs/running-tests) for more information.

### `yarn build`

Builds the app for production to the `build` folder.\
It correctly bundles React in production mode and optimizes the build for the best performance.

The build is minified and the filenames include the hashes.\
Your app is ready to be deployed!

See the section about [deployment](https://facebook.github.io/create-react-app/docs/deployment) for more information.


## Tests
This section is not available yet.
