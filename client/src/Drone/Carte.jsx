/* eslint-disable no-unused-vars */
import React, { useState, useRef, useEffect } from 'react';
import Map from '../Map';

const Carte = () => {
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
    }
    if (context) {
      map.updatePosition(posX, posY);
      // drone2.updatePosition();
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
    </>
  );
};

export default Carte;
