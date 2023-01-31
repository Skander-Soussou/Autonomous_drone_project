/* eslint-disable no-unused-expressions */
/* eslint-disable no-unused-vars */
import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { Button, Card } from 'semantic-ui-react';
import './State.css';
import { io } from 'socket.io-client';

const State = () => {
  const endpoint = 'http://localhost:5033';
  const socket = io(`${endpoint}`);
  const [droneUri] = useState(['radio://0/80/250K/E7E7E7E701', 'radio://0/21/2M/E7E7E7EE02']);
  const [drone1, setDrone1] = useState('');
  const [drone2, setDrone2] = useState('');
  const [batterie1, setBatterie1] = useState('');
  const [batterie2, setBatterie2] = useState('');

  const droneIdentify = async (data) => {
    try {
      const dataUri = data.substr(data.length - 2, data.length);
      const isConnected = await axios.get(`/state/${dataUri}`);
      let droneConnected = (Object.values(isConnected)[0]);
      droneConnected = Object.entries(droneConnected);
      droneConnected = droneConnected.join('');
      if (dataUri === 'E7') {
        setDrone2(droneConnected.substr(39, droneConnected.length));
        setBatterie2(droneConnected.substr(8, 10).substr(0, 2));
      } else {
        setBatterie1(droneConnected.substr(8, 10).substr(0, 2));
        setDrone1(droneConnected.substr(39, droneConnected.length));
      }
    } catch (err) {
      // console.error(err);
    }
  };

  const droneState = async () => {
    const state = await axios.get('/state/drone');
    setDrone1(state.data);
    setDrone2(state.data);
  };

  const identifyDrone = (data) => {
    axios.post('/start/identify/drone', { type: '2', data });
    droneState();
  };

  const message = () => {
    socket.on('message', (data) => {
      // console.log('data', data);
    });
  };

  const connectDrone = async (data) => {
    const response = await axios.post('/start/connect', { type: data });
    if (response.status === 201) droneIdentify(data);
  };

  const deconnectDrone = async (data) => {
    await axios.post('/disconnect', { type: data });
  };

  const startMission = async () => {
    await axios.post('/type/mission', { type: '0' });
    droneState();
  };

  const endMission = () => {
    axios.post('/finish/mission', { type: '1' });
    droneState();
  };

  return (
    <>
      <div className="card">
        <Card fluid>
          <Card.Content className="card">
            <Card.Header>Drone 1</Card.Header>
            <Card.Meta>
              uri :
              {' '}
              {droneUri[0]}
            </Card.Meta>
            <Card.Description>
              <strong>Batterie </strong>
              {' '}
              <span>
                :
                {' '}
                {batterie1}
              </span>
              {' '}
              <br />
              <strong>Etat</strong>
              {' '}
              <span>
                :
                {' '}
                {drone1}
                {' '}
              </span>
            </Card.Description>
          </Card.Content>
          <Card.Content extra>
            <div className="ui buttons">
              <Button inverted color="green" onClick={() => connectDrone(droneUri[0])}>
                Connecter
              </Button>
              <Button inverted color="red" onClick={() => identifyDrone(droneUri[0])}>
                Identifier
              </Button>
              <Button inverted color="blue" onClick={() => deconnectDrone(droneUri[0])}>
                Deconnecter
              </Button>
            </div>
          </Card.Content>
        </Card>
        <Card fluid>
          <Card.Content>
            <Card.Header>Drone 2</Card.Header>
            <Card.Meta>
              uri :
              {' '}
              {droneUri[1]}
            </Card.Meta>
            <Card.Description>
              <strong>Batterie </strong>
              {' '}
              <span>
                :
                {' '}
                {batterie2}
              </span>
              {' '}
              <br />
              <strong>Etat</strong>
              {' '}
              <span>
                :
                {' '}
                {drone2}
                {' '}
              </span>
            </Card.Description>
          </Card.Content>
          <Card.Content extra>
            <div className="ui buttons">
              <Button size="mini" inverted color="green" onClick={() => connectDrone(droneUri[1])}>
                Connecter
              </Button>
              <Button inverted color="red" onClick={() => identifyDrone(droneUri[1])}>
                Identifier
              </Button>
              <Button inverted color="blue" onClick={() => deconnectDrone(droneUri[1])}>
                Deconnecter
              </Button>
            </div>
          </Card.Content>
        </Card>
      </div>
      <div className="content">
        <button style={{ margin: '20px', fontFamily: 'monospace' }} className="ui button" type="button" onClick={startMission}>Démarrer mission</button>
        <button style={{ margin: '20px', fontFamily: 'monospace' }} className="ui button" type="button" onClick={endMission}>Terminer mission</button>
        <button style={{ margin: '20px', fontFamily: 'monospace' }} className="ui button" type="button" onClick={endMission}>Retour à la base</button>
      </div>
    </>
  );
};
export default State;
