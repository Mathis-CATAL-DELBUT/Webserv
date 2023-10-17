const sticksContainer = document.getElementById('sticks-container');
    const startButton = document.getElementById('startButton');
    const message = document.getElementById('message');
    const removeButton = document.getElementById('removeButton');
    let currentPlayer = 1;
    let sticksLeft = 0;
    let canRemoveSticks = false;
    let selectedSticks = 0;
    const maxSelectedSticks = 3;

    function createSticks(num) {
        sticksContainer.innerHTML = '';
        for (let i = 0; i < num; i++) {
            const stick = document.createElement('div');
            stick.classList.add('stick');
            stick.addEventListener('click', () => handleStickClick(stick));
            sticksContainer.appendChild(stick);
        }
    }

    function handleStickClick(stick) {
        if (canRemoveSticks) {
            if (stick.classList.contains('selected')) {
                stick.classList.remove('selected');
                selectedSticks -= 1;
            } else if (selectedSticks < maxSelectedSticks) {
                stick.classList.add('selected');
                selectedSticks += 1;
            }
            updateRemoveButton();
        }
    }

    function updateRemoveButton() {
        if (selectedSticks > 0) {
            removeButton.disabled = false;
        } else {
            removeButton.disabled = true;
        }
    }

    function setPlayerTurnColors() {
        if (currentPlayer === 1) {
            sticksContainer.classList.remove('player-2-turn');
            sticksContainer.classList.add('player-1-turn');
        } else {
            sticksContainer.classList.remove('player-1-turn');
            sticksContainer.classList.add('player-2-turn');
        }
    }

    startButton.addEventListener('click', function() {
        createSticks(21);
        startButton.disabled = true;
        message.textContent = `Player 1, it's your turn!`;
        sticksLeft = 21;
        currentPlayer = 1;
        canRemoveSticks = true;
        removeButton.disabled = true;
        selectedSticks = 0;
        setPlayerTurnColors();
    });

    removeButton.addEventListener('click', function() {
        if (canRemoveSticks && selectedSticks > 0) {
            sticksLeft -= selectedSticks;

            const selectedSticksArray = document.querySelectorAll('.stick.selected');
            selectedSticksArray.forEach((stick, index) => {
                if (index < selectedSticks) {
                    sticksContainer.removeChild(stick);
                }
            });

            if (sticksLeft === 0) {
                message.textContent = `Player ${currentPlayer} has lost!`;
                canRemoveSticks = false;
                resetButton.style.display = 'inline';
            } else {
                currentPlayer = (currentPlayer === 1) ? 2 : 1;
                message.textContent = `Player ${currentPlayer}, it's your turn!`;
                selectedSticks = 0;
                removeButton.disabled = true;
                canRemoveSticks = true;
                setPlayerTurnColors();
            }
        }
    });

    resetButton.addEventListener('click', function() {
        location.reload();
    });