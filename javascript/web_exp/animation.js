let positionX = 50;
let positionY = 50;
let speedX = 3;
let speedY = 4;
let animationId;
let isPlaying = false;
let bounceCount = 0;
let colorChangeCount = 0;
let speedLevel = 5;

const object = document.getElementById('animated-object');
const container = document.querySelector('.animation-container');
const containerRect = container.getBoundingClientRect();
const objectSize = 80;

function updatePosition() {
    // 更新位置
    positionX += speedX;
    positionY += speedY;
    
    // 边界检测
    if (positionX <= 0 || positionX + objectSize >= containerRect.width) {
        speedX = -speedX;
        changeColor();
        bounceCount++;
        document.getElementById('bounce-count').textContent = bounceCount;
    }
    
    if (positionY <= 0 || positionY + objectSize >= containerRect.height) {
        speedY = -speedY;
        changeShape();
        bounceCount++;
        document.getElementById('bounce-count').textContent = bounceCount;
    }
    
    // 应用新位置
    object.style.left = positionX + 'px';
    object.style.top = positionY + 'px';
    
    // 继续动画
    animationId = requestAnimationFrame(updatePosition);
}

function changeColor() {
    const hue = Math.floor(Math.random() * 360);
    object.style.background = `linear-gradient(45deg, hsl(${hue}, 100%, 60%), hsl(${(hue + 120) % 360}, 100%, 40%))`;
    colorChangeCount++;
    document.getElementById('color-change').textContent = colorChangeCount;
}

function changeShape() {
    const isRound = Math.random() > 0.5;
    object.style.borderRadius = isRound ? '50%' : '10px';
    object.style.transform = `scale(${0.8 + Math.random() * 0.4})`;
}

function startAnimation() {
    if (!isPlaying) {
        isPlaying = true;
        updatePosition();
    }
}

function pauseAnimation() {
    if (isPlaying) {
        isPlaying = false;
        cancelAnimationFrame(animationId);
    }
}

function resetAnimation() {
    pauseAnimation();
    positionX = 50;
    positionY = 50;
    object.style.left = positionX + 'px';
    object.style.top = positionY + 'px';
    bounceCount = 0;
    colorChangeCount = 0;
    document.getElementById('bounce-count').textContent = bounceCount;
    document.getElementById('color-change').textContent = colorChangeCount;
    object.style.borderRadius = '50%';
    object.style.transform = 'scale(1)';
    object.style.background = 'linear-gradient(45deg, #ff00cc, #333399)';
    speedLevel = 5;
    document.getElementById('speed-value').textContent = speedLevel;
    const speedFactor = speedLevel / 5;
    speedX = 3 * speedFactor;
    speedY = 4 * speedFactor;
}

function changeSpeed(direction) {
    if (direction === 'up' && speedLevel < 10) {
        speedLevel++;
    } else if (direction === 'down' && speedLevel > 1) {
        speedLevel--;
    }
    
    const speedFactor = speedLevel / 5;
    speedX = Math.sign(speedX) * (3 * speedFactor);
    speedY = Math.sign(speedY) * (4 * speedFactor);
    
    document.getElementById('speed-value').textContent = speedLevel;
}

// 事件监听器
document.getElementById('start-btn').addEventListener('click', startAnimation);
document.getElementById('pause-btn').addEventListener('click', pauseAnimation);
document.getElementById('reset-btn').addEventListener('click', resetAnimation);
document.getElementById('speed-up').addEventListener('click', () => changeSpeed('up'));
document.getElementById('speed-down').addEventListener('click', () => changeSpeed('down'));

// 初始设置
resetAnimation();