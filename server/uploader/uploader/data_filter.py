

class DataFilter:
    def __init__(self):
        pass

    def filter(self, filter_func):
        return filter(filter_func, self.data)