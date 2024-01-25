# LAB: Perform Foundational Infrastructure Tasks in Google Cloud

## 1. Cloud Functions

!!! abstract "Objects"
    - Create a cloud function
    - Deploy and test the function
    - View logs

### Cloud function with consoles

🔘 Step1: console -> Cloud Function -> create function -> edit setting
🔘 Step2: write the function -> deploy
🔘 Step3: testing -> write test http request -> send tests
🔘 Step4: view logging -> the log history that displays in Query results

### Cloud function with command line

🔘 Step1: create a function. This is just normal linux operation.

```shell
gcloud config set compute/region us-central1  # set the default region
mkdir gcf_hello_world
cd gcf_hello_world
nano index.js
```

🔘 Step2: create a cloud bucket

`mb` stands for "make bucket". The second varibale is `gs://[bucket_name]`, here we use project id as bucket name.

```shell
gsutil mb -p $GOOGLE_CLOUD_PROJECT gs://$GOOGLE_CLOUD_PROJECT
```

🔘 Step3: deploy function

- use `gcloud functions deploy` to deploy function with given bucket, trigger and runtime setting.
- use `gcloud functions describe` to vie the state of function

```shell
gcloud functions deploy helloWorld \
--stage-bucket $GOOGLE_CLOUD_PROJECT \
--trigger-topic hello_world \
--runtime nodejs20

gcloud functions describe helloWorld
```

🔘 Step4: test function
  
use `gcloud functions call` to test function.

```shell
DATA=$(printf 'Hello World!'|base64) && gcloud functions call helloWorld --data '{"data":"'$DATA'"}'
```

🔘 Step5: view logs

use `gcloud functions logs read` to read the logs of a function.

```shell
gcloud functions logs read helloWorld
```

## 2. Pub/Sub

!!! abstract "Objects"
    - Set up a topic to hold data.
    - Subscribe to a topic to access the data.
    - Publish and then consume messages with a pull subscriber.

### Pub/Sub from console

🔘 Step 1 setting up pub/sub:  Go to pub/sub -> create topic

🔘 Step 2 add a subscription: Go to pub/sub > Topics -> create subscription

🔘 Step 3 public a message to topic: Go to pub/sub > Topics

🔘 Step 4: view the message: Go to subscription -> message -> bulic

### Pub/Sub from command line

🔘 Step 1: setting up pub/sub

```shell
gcloud pubsub topics create myTopic
gcloud pubsub topics create Test1
gcloud pubsub topics create Test2
gcloud pubsub topics list
```

🔘 Step 2: add a subscription

```shell
gcloud pubsub subscriptions create --topic myTopic mySubscription
gcloud pubsub subscriptions delete myTopic

gcloud pubsub topics list-subscriptions myTopic
```

🔘 Step 3: publish and pull a message to/from topic

To publish a message to a topic, use `gcloud pubsub topics publish [topic] -- message [msg]`.

```shell
gcloud pubsub topics publish myTopic --message "Hello"
gcloud pubsub topics publish myTopic --message "Publisher's name is <YOUR NAME>"
```

To pull **a single** msg, use the flowing command. Once an individual message has been outputted from a particular subscription-based pull command, you cannot access that message again with the pull command.

```shell
gcloud pubsub subscriptions pull mySubscription --auto-ack
```

🔘 Step 4: pulling all messages

To pull **multiple messages**, use `pull --limit [number]`.

```shell
gcloud pubsub subscriptions pull mySubscription --auto-ack --limit=3
```

### Pub/Sub from API(Python)

🔘 Step 1:  Create a virtual environment

Python virtual environments are used to isolate package installation from the system.

```shell
sudo apt-get install -y virtualenv
python3 -m venv venv
source venv/bin/activate
```

🔘 Step 2: Install the client library

```shell
pip install --upgrade google-cloud-pubsub
git clone https://github.com/googleapis/python-pubsub.git
cd python-pubsub/samples/snippets
```

🔘 Step 3: Create a topic

```shell
echo $GOOGLE_CLOUD_PROJECT
cat publisher.py
python publisher.py -h
python publisher.py $GOOGLE_CLOUD_PROJECT create MyTopic
```

Topic is created using `pubsub_v1.PublisherClient().create_topic(topic_name)`,  `topic_name` should follow the format below.

```python
import os
from google.cloud import pubsub_v1

publisher = pubsub_v1.PublisherClient()
topic_name = 'projects/{project_id}/topics/{topic}'.format(
    project_id=os.getenv('GOOGLE_CLOUD_PROJECT'),
    topic='MY_TOPIC_NAME',  # Set this to something appropriate.
)
publisher.create_topic(name=topic_name)
```

🔘 Step 4: Create a subscription

```shell
python subscriber.py $GOOGLE_CLOUD_PROJECT create MyTopic MySub
python subscriber.py $GOOGLE_CLOUD_PROJECT list-in-project
```

Subscription is created/subscribe by these api calls.

```python
with pubsub_v1.SubscriberClient() as subscriber:
  subscriber.create_subscription(
      name=subscription_name, topic=topic_name)
  future = subscriber.subscribe(subscription_name, callback)
```

🔘 Step 5: Publish messages

```shell
gcloud pubsub topics publish MyTopic --message "Hello"
gcloud pubsub topics publish MyTopic --message "Publisher's name is <YOUR NAME>"
gcloud pubsub topics publish MyTopic --message "Publisher likes to eat <FOOD>"
```

Message is published using `pubsub_v1.PublisherClient().publish(topic_name, msg, spam)`.

```python
future = publisher.publish(topic_name, b'My first message!', spam='eggs')
future.result()
```

🔘 Step 6. View messages

```shell
python subscriber.py $GOOGLE_CLOUD_PROJECT receive MySub
```

The message can be viewed using the result of `subscriber.subscribe().result()`.

```python
 streaming_pull_future = subscriber.subscribe(subscription_path, callback=callback)
    print(f"Listening for messages on {subscription_path}..\n")

    # Wrap subscriber in a 'with' block to automatically call close() when done.
    with subscriber:
        try:
            # When `timeout` is not set, result() will block indefinitely,
            # unless an exception is encountered first.
            streaming_pull_future.result(timeout=timeout)
        except TimeoutError:
            streaming_pull_future.cancel()  # Trigger the shutdown.
            streaming_pull_future.result()  # Block until the shutdown is complete.
```
