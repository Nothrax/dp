class Message:
    def __init__(self, company: str, gateway_id: str) -> None:
        self._json_message = ""
        self.company = company
        self.gateway_id = gateway_id
        return

    def parse_message(self, json_message: str) -> None:
        self._json_message = json_message
        print(f"Message received from compay: {self.company}, gateway {self.gateway_id} and json_message: {self._json_message}")

    def get_output_protocol_message(self) -> str:
        return "{}"
