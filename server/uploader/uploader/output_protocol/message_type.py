from enum import Enum


class MessageType(Enum):
    DATA = 1
    DATA_ACK = 2
    DATA_READ = 3
    DATA_READ_RESPONSE = 4
    DATA_READ_ACK = 5
