//authors: Lily Lin and Alicia Deng

let cols, rows;
let spacing = 15; // distance between squares
let t = 0; // time variable
let lastHitX, lastHitY;

function setup() {
  createCanvas(windowWidth, windowHeight);
  noStroke();
  rectMode(CENTER);
  cols = width / spacing;
  rows = height / spacing;
  // cols and rows make the screen into a grid
  lastHitX = random(width);
  lastHitY = random(height);
}

//this will generate a elliptical vibrating pattern
function draw() {
  background(20);
  
  // Speed up dynamically based on common screen sizes; more lively vibrations on larger screens
  let speedFactor = 1.0; //smaller screens like phones and some tablets
  if (windowWidth > 1500) speedFactor = 1.5; //big screens like laptop 
  else if (windowWidth > 900) speedFactor = 1.3; //ultra big screens like monitor/desktop

  //create a random speed of growth with the noise distribution
  t += map(noise(0.01), -1, 1, 0.01, 0.05) * speedFactor;

  //create the grid
  for (let i = 0; i < cols; i++) {
    for (let j = 0; j < rows; j++) {
      let x = i * spacing + spacing / 2;
      let y = j * spacing + spacing / 2;

      let dx = x - width / 2;
      let dy = y - height / 2; // set to circular dimensions

      // range of the ellipse
      dx /= width / 2;
      dy /= height / 2;
      let distFromCenter = sqrt(dx * dx + dy * dy);

      let ellipseScale = width / height;
      
      //falloff means the circles in the center are larger than circles on the edges
      let falloff = 1 - (distFromCenter * ellipseScale) / 2; // falloff = 1 in center to 0 at edge (distFromCenter / maxDistance)
      falloff = constrain(falloff, 0.4, 0.7); // clamp so it never goes negative

      let angle = distFromCenter * 5 - t;
      let wave = noise(angle);

      //color modifications
      let r = map(noise(i * 0.1, j * 0.1, t), 0, 1, 100, 255);
      let g = map(noise(i * 0.1 + 100, j * 0.1, t), 0, 1, 100, 255);
      let b = map(noise(i * 0.1, j * 0.1 + 100, t), 0, 1, 100, 255);

      let pixelSize = map(wave, 0.5, 0.7, 0, spacing) * pow(falloff, 2);
      
      //when the cicles hit the edge, creates a "glow" effect by growing large
      let edgeThreshold = 50;
      if (x < edgeThreshold || x > width - edgeThreshold || 
          y < edgeThreshold || y > height - edgeThreshold) {
        r = min(r + 80, 255);
        g = min(g + 80, 255);
        b = min(b + 80, 255);
        pixelSize *= 1.8;
      }

      //draw the image
      fill(r, g, b);
      ellipse(x, y, pixelSize, pixelSize);
    }
  }
}
