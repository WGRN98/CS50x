--SELECT AVG(energy)
--FROM songs
--WHERE artist_id =
--(
--    SELECT id
--    FROM artists
--    WHERE name = 'Drake'
--);

-- The average energy of songs that are by Drake
SELECT AVG(energy)
FROM songs
JOIN artists ON songs.artist_id = artists.id
WHERE artists.name = 'Drake';
