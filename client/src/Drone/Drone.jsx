import React from 'react';
import './Drone.css';
import { Button } from 'semantic-ui-react';
import State from './State';
import Carte from './Carte';

function Drone() {
  return (
    <div className="drone">
      <State />
      <>
        {/* <button type="button">Lancer logs</button> */}
        <Button
          inverted
          color="green"
          style={{ margin: '18px', fontFamily: 'lucida grande' }}
          type="button"
          size="large"
        >
          Lancer logs
        </Button>

        {/* <button type="button">Arreter logs</button> */}
        <Button
          inverted
          color="red"
          style={{ margin: '18px', fontFamily: 'lucida grande' }}
          type="button"
          size="large"
        >
          Arreter logs
        </Button>

      </>
      <div className="carte">
        <Carte />
      </div>
    </div>
  );
}

export default Drone;
