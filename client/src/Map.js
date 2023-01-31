export default class Map {
  constructor(x, y, speed, ctx, canvas) {
    this.x = x;
    this.y = y;
    this.speed = speed;
    this.sizeX = 50;
    this.sizeY = 50;
    this.context = ctx;
    this.canvas = canvas;
  }

  draw(x, y) {
    this.context.beginPath();
    this.context.rect(x, y, this.sizeX, this.sizeY);
    this.context.fillStyle = 'blue';
    this.context.fill();
  }

  update() {
    this.context.clearRect(0, 0, 800, 700);
    this.x += Math.random();
    this.y += Math.random() / 3;
    this.draw(this.x, this.y);
  }

  updatePosition() {
    requestAnimationFrame(this.updatePosition.bind(this));
    this.update();
  }
}
