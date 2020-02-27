# Cryptocurrency Recommendations with Machine Learning Techniques
In this project we are given the task of training a model, using supervised and unsupervised learning for cryptocurrency recommendations. 

The project is seperated in three parts:
1. **Supervised Learning: Nearest Neighbor and Range Search using LSH and HyperCube (metrics used: cosine similarity, euclidean distance)**
    * Given a set of d-diamentional vectors we train a model that finds the Nearest Neighbor and does a Range Search given a set of queries, using the approximation methos: LSH and Hypercube. Those results are then compared with their brute-force counterpart.

1. **Unsupervised Learning: Kmeans and Kmedoid Clustering with different Initialization, Assignment and Update options. (with Silhouette)**
    * Given a set of d-diamentional vectors we train a model that finds clusters given a set of options include:
      * Initialization
        - Random selection
        - K-means++
      * Assignment
        - Lloyd's assignment
        - Assignment by Range Search with LSH
        - Assignment by Range Search with Hypercube
      * Update
        - K-means
        - Partitioning Around Medoids
       

1. **Cryptocurrency Recommendations using LSH Range Search and Clustering (with Cross Validation)**
    * Given a set of Tweets(tokenized based on lexicon L) and a set of Users, we analyse the "emotion" of each tweet based on lexicon A (words=emotion rating), that emotion can be 'positive', 'negative', or 'neutral' in the range of: [-1, 1]. Lexicon K contains the cryptocurrencies being considered.
    * We calculate each users emotion to each cryptocurrency based on his tweets and recommend to him 5 new cryptocurrencies.
    * For each clusters (created in the previous part of the project) we calculate the total "emotion" to each cryptocurrency. Then we recommend 2 new cryptocurrencies to the users of said cluster. 