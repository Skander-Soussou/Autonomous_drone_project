import React from 'react';
import 'semantic-ui-css/semantic.min.css';
import { Button } from 'semantic-ui-react';
import {
  BrowserRouter as Router,
  Switch,
  Route,
  Link,
} from 'react-router-dom';
// import { exec } from 'child_process';
import axios from 'axios';
import AnimationDrone from '../AnimationDrone/AnimationDrone';
import './App.css';
import Drone from '../Drone/Drone';
import Argos from '../Argos-Simulation/Argos';

const App = () => {
  const startArgos = () => {
    axios.post('http://localhost:5001/runArgos');
  };

  return (
    <div className="app">
      <Router>
        <Switch>
          <Route exact path="/">
            <AnimationDrone />
            <div className="button">
              <Link to="/drone"><Button size="big" className="drone-button">Drone Physique</Button></Link>
              <Link to="/argos"><Button onClick={startArgos} size="big" className="argos-button" style={{ color: 'rgba(0,0,0,0.65)' }} color="red">Argos Simulation</Button></Link>
            </div>
          </Route>
          <Route path="/drone">
            <Drone />
          </Route>
          <Route path="/argos">
            <Argos />
          </Route>
        </Switch>
      </Router>
    </div>
  );
};

export default App;
