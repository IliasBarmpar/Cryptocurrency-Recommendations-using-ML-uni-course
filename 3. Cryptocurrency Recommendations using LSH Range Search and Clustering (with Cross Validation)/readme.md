## Cryptocurrency Recommendations using LSH Range Search and Clustering (with Cross Validation)
-	Given a set of Tweets(tokenized based on lexicon L) and a set of Users, we analyse the "emotion" of each tweet based on lexicon A (words=emotion rating), that emotion can be 'positive', 'negative', or 'neutral' in the range of: [-1, 1]. Lexicon K contains the cryptocurrencies being considered.

-	We calculate each users emotion to each cryptocurrency based on his tweets and recommend to him 5 new cryptocurrencies.

-	For each clusters (created in the previous part of the project) we calculate the total "emotion" to each cryptocurrency. Then we recommend 2 new cryptocurrencies to the users of said cluster.
