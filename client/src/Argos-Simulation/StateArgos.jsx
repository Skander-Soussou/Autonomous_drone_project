/* eslint-disable no-unused-expressions */
/* eslint-disable no-unused-vars */
import React, { useState, useEffect } from 'react';
import { Button, Card } from 'semantic-ui-react';
import './Argos.css';
import './StateArgos.css';

const StateArgos = () => {
  const [drone1, setDrone1] = useState('');
  const [drone2, setDrone2] = useState('');
  const [batterie1, setBatterie1] = useState('');
  const [batterie2, setBatterie2] = useState('');

  const droneIdentify = async (data) => {

  };

  const droneState = async () => {

  };

  const identifyDrone = (data) => {

  };

  const message = () => {

  };

  const connectDrone = async (data) => {

  };

  const deconnectDrone = async (data) => {
  };

  const startMission = async () => {

  };

  const endMission = () => {

  };

  return (
    <>
      <div className="card">
        <Card fluid>
          <Card.Content className="card">
            <Card.Header>Drone 1</Card.Header>
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
              <Button inverted color="green">
                Connecter
              </Button>
              <Button inverted color="red">
                Identifier
              </Button>
              <Button inverted color="blue">
                Deconnecter
              </Button>
            </div>
          </Card.Content>
        </Card>
        <Card fluid>
          <Card.Content>
            <Card.Header>Drone 2</Card.Header>
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
              <Button size="mini" inverted color="green">
                Connecter
              </Button>
              <Button inverted color="red">
                Identifier
              </Button>
              <Button inverted color="blue">
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
export default StateArgos;
