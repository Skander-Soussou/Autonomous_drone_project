/* eslint-disable no-unused-vars */
import React, { useState, useRef, useEffect } from 'react';
import './Argos.css';
import Map from '../Map';
import StateArgos from './StateArgos';

const Argos = () => {
  const canvasRef = useRef(null);
  const [context, setContext] = useState(null);
  const [posX, setPosX] = useState(0);
  const [posY, setPosY] = useState(0);

  useEffect(() => {
    let map = null;
    // let drone2 = null;
    const renderCtx = canvasRef.current.getContext('2d');
    if (renderCtx) {
      setContext(renderCtx);
      map = new Map(0, 0, 1, context, canvasRef.current);
      // drone2 = new Map(50, 50, 5, context, canvasRef.current);
    }
    if (context) {
      map.updatePosition(posX, posY);
    }
  }, [context]);

  return (
    <>
      <canvas
        id="canvas"
        ref={canvasRef}
        width={800}
        height={700}
        style={{
          border: '1px solid #000',
          marginTop: 150,
          backgroundColor: '#FFFF',
        }}
      />
      <StateArgos />
    </>
  );
};

export default Argos;
