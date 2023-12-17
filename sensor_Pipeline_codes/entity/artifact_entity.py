from dataclasses import dataclass



@dataclass
class DataInestionArtifact:
    trained_file_path: str
    test_file_path: str

