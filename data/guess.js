const targetNumber = Math.floor(Math.random() * 101);

let lives = 7;

const guessInput = document.getElementById('guessInput');
const guessButton = document.getElementById('guessButton');
const resetButton = document.getElementById('resetButton');
const message = document.getElementById('message');

function checkGuess() {
    const userGuess = parseInt(guessInput.value);
    
    if (isNaN(userGuess) || userGuess < 0 || userGuess > 100) {
        message.textContent = "Please enter a valid number between 0 and 100.";
        return;
    }
    lives--;
    if (userGuess === targetNumber) {
        message.textContent = `Congratulations! The number was ${targetNumber}.`;
        endGame();
    } else if (lives === 0) {
        message.textContent = `You've used up all your lives. The number was ${targetNumber}.`;
        endGame();
    } else if (userGuess < targetNumber) {
        message.textContent = `Is more! You have ${lives} left.`;
    } else {
        message.textContent = `Is less! You have ${lives} left.`;
    }
    guessInput.value = '';
}

function endGame() {
    guessButton.disabled = true;
    resetButton.style.display = 'inline';
}

resetButton.addEventListener('click', function() {
    location.reload();
});

guessButton.addEventListener('click', checkGuess);
