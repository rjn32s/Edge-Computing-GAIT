from sensor.exception import SensorException
from sensor.logger import logging
from sensor.entity.config_entity import DataIngestionConfig
from sensor.entity.artifact_entity import DataInestionArtifact
from sklearn.model_selection import train_test_split
import os,sys
from pandas import DataFrame
from sensor.data_access.sensor_data import SensorData
from sensor.utils.main_utils import read_yaml_file
from sensor.constant.training_pipeline import SCHEMA_FILE_PATH


class DataIngestion:
    

    def __init__(self,data_ingestion_config: DataIngestionConfig) -> None:
        

        try:
            self.data_ingestion_config = data_ingestion_config
            self._schema_config = read_yaml_file(SCHEMA_FILE_PATH)
        
        except Exception as e:
            raise SensorException(e,sys)

        
        def export_data_into_feature_store(self) -> DataFrame:


            """
            Export mongo db collection as data Frame into the feature 
            
            """

            try:
                logging.info("Exporting data from mongodb to feature store")

                sensor_data = SensorData()
                dataframe = sensor_data.export_collection_as_datarame()

            except Exception as e:
                raise SensorException(e,sys)


        def split_data_as_train_test(self, dataFrame: DataFrame) -> None:

            """
            Feature store dataset will be spillited into  train and test file
         
            """

            try:
                train_set , test_set = train_test_split(
                    dataFrame , test_size =  self.data_ingestion_config.train_test_split_ration
                )

                logging.info("Performed train test split on the dataframe")
                logging.info(
                "Exited split_data_as_train_test method of Data_Ingestion class"
                             )
                train_set.to_csv(
                    self.data_ingestion_config.training_file_path , index = False , header = True
                )

                test_set.info("Exported train and test file path.")
            except Exception as e:
                raise SensorData(e,sys)

            
        def initiate_data_ingestion(self) -> DataInestionArtifact:

            try:

                dataframe = self.export_data_into_feature_store()
                dataframe = dataframe.drop(self._schema_config["drop_columns"],axis =1 )
                self.split_data_as_train_test(dataframe = dataframe)

                data_ingestion_artifact = DataInestionArtifact(trained_file_path = self.data_ingestion_config.training_file_path,
                test_file_path = self.data_ingestion_config.testing_file_path)

                return data_ingestion_artifact
            except Exception as e:
                raise SensorException(e,sys)